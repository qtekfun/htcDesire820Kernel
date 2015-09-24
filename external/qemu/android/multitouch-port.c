/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "qemu-common.h"
#include "utils/panic.h"
#include "android/hw-events.h"
#include "android/charmap.h"
#include "android/multitouch-screen.h"
#include "android/sdk-controller-socket.h"
#include "android/multitouch-port.h"
#include "android/globals.h"  
#include "android/opengles.h"
#include "android/utils/misc.h"
#include "android/utils/jpeg-compress.h"
#include "android/utils/debug.h"

#define  E(...)    derror(__VA_ARGS__)
#define  W(...)    dwarning(__VA_ARGS__)
#define  D(...)    VERBOSE_PRINT(mtport,__VA_ARGS__)
#define  D_ACTIVE  VERBOSE_CHECK(mtport)

#define TRACE_ON    1

#if TRACE_ON
#define  T(...)    VERBOSE_PRINT(mtport,__VA_ARGS__)
#else
#define  T(...)
#endif

#define SDKCTL_MT_TIMEOUT      3000


#define SDKCTL_MT_MOVE                  1
#define SDKCTL_MT_FISRT_DOWN            2
#define SDKCTL_MT_LAST_UP               3
#define SDKCTL_MT_POINTER_DOWN          4
#define SDKCTL_MT_POINTER_UP            5
#define SDKCTL_MT_FB_UPDATE             6
#define SDKCTL_MT_FB_UPDATE_RECEIVED    7
#define SDKCTL_MT_FB_UPDATE_HANDLED     8

struct AndroidMTSPort {
    
    void*               opaque;
    
    SDKCtlSocket*       sdkctl;
    
    AJPEGDesc*          jpeg_compressor;
    
    SDKCtlDirectPacket* fb_packet;
};

typedef struct QueryDispData {
    
    int     width;
    
    int     height;
} QueryDispData;

typedef struct AndroidMTEvent {
    
    int     pid;
    
    int     x;
    
    int     y;
    
    int     pressure;
} AndroidMTEvent;

typedef struct AndroidMTPtr {
    
    int     pid;
} AndroidMTPtr;

static void
_mts_port_free(AndroidMTSPort* mtsp)
{
    if (mtsp != NULL) {
        if (mtsp->fb_packet != NULL) {
            sdkctl_direct_packet_release(mtsp->fb_packet);
        }
        if (mtsp->jpeg_compressor != NULL) {
            jpeg_compressor_destroy(mtsp->jpeg_compressor);
        }
        if (mtsp->sdkctl != NULL) {
            sdkctl_socket_release(mtsp->sdkctl);
        }
        AFREE(mtsp);
    }
}



static void
_on_action_down(int tracking_id, int x, int y, int pressure)
{
    multitouch_update_pointer(MTES_DEVICE, tracking_id, x, y, pressure);
}

static void
_on_action_up(int tracking_id)
{
    multitouch_update_pointer(MTES_DEVICE, tracking_id, 0, 0, 0);
}

static void
_on_action_pointer_down(int tracking_id, int x, int y, int pressure)
{
    multitouch_update_pointer(MTES_DEVICE, tracking_id, x, y, pressure);
}

static void
_on_action_pointer_up(int tracking_id)
{
    multitouch_update_pointer(MTES_DEVICE, tracking_id, 0, 0, 0);
}

static void
_on_action_move(int tracking_id, int x, int y, int pressure)
{
    multitouch_update_pointer(MTES_DEVICE, tracking_id, x, y, pressure);
}


static void
_on_move(const AndroidMTEvent* param, int pointers_count)
{
    int n;
    for (n = 0; n < pointers_count; n++, param++) {
        T("Multi-touch: MOVE(%d): %d-> %d:%d:%d",
          n, param->pid, param->x, param->y, param->pressure);
         _on_action_move(param->pid, param->x, param->y, param->pressure);
    }
}

static void
_on_down(const AndroidMTEvent* param)
{
    T("Multi-touch: 1-ST DOWN: %d-> %d:%d:%d",
      param->pid, param->x, param->y, param->pressure);
    _on_action_down(param->pid, param->x, param->y, param->pressure);
}

static void
_on_up(const AndroidMTPtr* param)
{
    T("Multi-touch: LAST UP: %d", param->pid);
    _on_action_up(param->pid);
}

static void
_on_pdown(const AndroidMTEvent* param)
{
    T("Multi-touch: DOWN: %d-> %d:%d:%d",
      param->pid, param->x, param->y, param->pressure);
    _on_action_pointer_down(param->pid, param->x, param->y, param->pressure);
}

static void
_on_pup(const AndroidMTPtr* param)
{
    T("Multi-touch: UP: %d", param->pid);
    _on_action_pointer_up(param->pid);
}


static AsyncIOAction
_on_multitouch_socket_connection(void* opaque,
                                 SDKCtlSocket* sdkctl,
                                 AsyncIOState status)
{
    if (status == ASIO_STATE_FAILED) {
        
        if (sdkctl_socket_is_handshake_ok(sdkctl)) {
            sdkctl_socket_reconnect(sdkctl, SDKCTL_DEFAULT_TCP_PORT,
                                    SDKCTL_MT_TIMEOUT);
        }
    }
    return ASIO_ACTION_DONE;
}

static void
_on_multitouch_port_connection(void* opaque,
                               SDKCtlSocket* sdkctl,
                               SdkCtlPortStatus status)
{
    switch (status) {
        case SDKCTL_PORT_CONNECTED:
            D("Multi-touch: SDK Controller is connected");
            break;

        case SDKCTL_PORT_DISCONNECTED:
            D("Multi-touch: SDK Controller is disconnected");
            
            if (android_hw->hw_gpu_enabled) {
                android_setPostCallback(NULL, NULL);
            }
            break;

        case SDKCTL_PORT_ENABLED:
            D("Multi-touch: SDK Controller port is enabled.");
            
            if (android_hw->hw_gpu_enabled) {
                android_setPostCallback(multitouch_opengles_fb_update, NULL);
            }
            
            multitouch_refresh_screen();
            break;

        case SDKCTL_PORT_DISABLED:
            D("Multi-touch: SDK Controller port is disabled.");
            
            if (android_hw->hw_gpu_enabled) {
                android_setPostCallback(NULL, NULL);
            }
            break;

        case SDKCTL_HANDSHAKE_CONNECTED:
            D("Multi-touch: Handshake succeeded with connected port.");
            break;

        case SDKCTL_HANDSHAKE_NO_PORT:
            D("Multi-touch: Handshake succeeded with disconnected port.");
            break;

        case SDKCTL_HANDSHAKE_DUP:
            W("Multi-touch: Handshake failed due to port duplication.");
            sdkctl_socket_disconnect(sdkctl);
            break;

        case SDKCTL_HANDSHAKE_UNKNOWN_QUERY:
            W("Multi-touch: Handshake failed due to unknown query.");
            sdkctl_socket_disconnect(sdkctl);
            break;

        case SDKCTL_HANDSHAKE_UNKNOWN_RESPONSE:
        default:
            W("Multi-touch: Handshake failed due to unknown reason.");
            sdkctl_socket_disconnect(sdkctl);
            break;
    }
}

static void
_on_multitouch_message(void* client_opaque,
                       SDKCtlSocket* sdkctl,
                       SDKCtlMessage* message,
                       int msg_type,
                       void* msg_data,
                       int msg_size)
{
    switch (msg_type) {
        case SDKCTL_MT_MOVE: {
            assert((msg_size / sizeof(AndroidMTEvent)) && !(msg_size % sizeof(AndroidMTEvent)));
            _on_move((const AndroidMTEvent*)msg_data, msg_size / sizeof(AndroidMTEvent));
            break;
        }

        case SDKCTL_MT_FISRT_DOWN:
            assert(msg_size / sizeof(AndroidMTEvent) && !(msg_size % sizeof(AndroidMTEvent)));
            _on_down((const AndroidMTEvent*)msg_data);
            break;

        case SDKCTL_MT_LAST_UP:
            _on_up((const AndroidMTPtr*)msg_data);
            break;

        case SDKCTL_MT_POINTER_DOWN:
            assert(msg_size / sizeof(AndroidMTEvent) && !(msg_size % sizeof(AndroidMTEvent)));
            _on_pdown((const AndroidMTEvent*)msg_data);
            break;

        case SDKCTL_MT_POINTER_UP:
            _on_pup((const AndroidMTPtr*)msg_data);
            break;

        case SDKCTL_MT_FB_UPDATE_RECEIVED:
            D("Framebuffer update ACK.");
            break;

        case SDKCTL_MT_FB_UPDATE_HANDLED:
            D("Framebuffer update handled.");
            multitouch_fb_updated();
            break;

        default:
            W("Multi-touch: Unknown message %d", msg_type);
            break;
    }
}


AndroidMTSPort*
mts_port_create(void* opaque)
{
    AndroidMTSPort* mtsp;

    ANEW0(mtsp);
    mtsp->opaque                = opaque;

    
    multitouch_init(mtsp);

    mtsp->jpeg_compressor =
        jpeg_compressor_create(sdkctl_message_get_header_size() + sizeof(MTFrameHeader), 4096);

    mtsp->sdkctl = sdkctl_socket_new(SDKCTL_MT_TIMEOUT, "multi-touch",
                                     _on_multitouch_socket_connection,
                                     _on_multitouch_port_connection,
                                     _on_multitouch_message, mtsp);
    sdkctl_init_recycler(mtsp->sdkctl, 64, 8);

    mtsp->fb_packet = sdkctl_direct_packet_new(mtsp->sdkctl);

    
    sdkctl_socket_connect(mtsp->sdkctl, SDKCTL_DEFAULT_TCP_PORT,
                          SDKCTL_MT_TIMEOUT);

    return mtsp;
}

void
mts_port_destroy(AndroidMTSPort* mtsp)
{
    _mts_port_free(mtsp);
}


static void
_fb_compress(const AndroidMTSPort* mtsp,
             const MTFrameHeader* fmt,
             const uint8_t* fb,
             int jpeg_quality,
             int ydir)
{
    T("Multi-touch: compressing %d bytes frame buffer", fmt->w * fmt->h * fmt->bpp);

    jpeg_compressor_compress_fb(mtsp->jpeg_compressor, fmt->x, fmt->y, fmt->w,
                                fmt->h, fmt->disp_height, fmt->bpp, fmt->bpl,
                                fb, jpeg_quality, ydir);
}

int
mts_port_send_frame(AndroidMTSPort* mtsp,
                    MTFrameHeader* fmt,
                    const uint8_t* fb,
                    on_sdkctl_direct_cb cb,
                    void* cb_opaque,
                    int ydir)
{
    
    if (!sdkctl_socket_is_port_ready(mtsp->sdkctl)) {
        return -1;
    }

    
    fmt->format = MTFB_JPEG;
    _fb_compress(mtsp, fmt, fb, 10, ydir);

    
    const int update_size =
        sizeof(MTFrameHeader) + jpeg_compressor_get_jpeg_size(mtsp->jpeg_compressor);

    uint8_t* const msg = (uint8_t*)jpeg_compressor_get_buffer(mtsp->jpeg_compressor);

    
    sdkctl_init_message_header(msg, SDKCTL_MT_FB_UPDATE, update_size);

    
    memcpy(msg + sdkctl_message_get_header_size(), fmt, sizeof(MTFrameHeader));

    
    const float comp_rate = ((float)jpeg_compressor_get_jpeg_size(mtsp->jpeg_compressor) / (fmt->w * fmt->h * fmt->bpp)) * 100;

    fmt->x = fmt->y = fmt->w = fmt->h = 0;

    
    sdkctl_direct_packet_send(mtsp->fb_packet, msg, cb, cb_opaque);

    T("Multi-touch: Sent %d bytes in framebuffer update. Compression rate is %.2f%%",
      update_size, comp_rate);

    return 0;
}
