/* Copyright (C) 2010 The Android Open Source Project
**
** This software is licensed under the terms of the GNU General Public
** License version 2, as published by the Free Software Foundation, and
** may be copied, distributed, and modified under those terms.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/


#include "android/utils/system.h"
#include "android/utils/debug.h"
#include "android/utils/panic.h"
#include "android/sync-utils.h"
#include "android/protocol/core-connection.h"
#include "android/protocol/fb-updates.h"
#include "android/protocol/fb-updates-impl.h"

typedef enum FbImplState {
    
    EXPECTS_HEADER,

    
    EXPECTS_PIXELS,
} FbImplState;

typedef struct FrameBufferImpl {
    
    QFrameBuffer*   fb;

    
    CoreConnection* core_connection;

    
    FBUpdateMessage update_header;

    
    uint8_t*        reader_buffer;

    
    size_t          reader_offset;

    
    size_t          reader_bytes;

    
    FbImplState     fb_state;

    
    int             sock;

    
    LoopIo          io[1];

    
    int             bits_per_pixel;
} FrameBufferImpl;

static FrameBufferImpl _fbImpl;

static void
_update_rect(QFrameBuffer* fb, uint16_t x, uint16_t y, uint16_t w, uint16_t h,
             uint8_t bits_per_pixel, uint8_t* pixels)
{
    if (fb != NULL) {
        uint16_t n;
        const uint8_t* src = pixels;
        const uint16_t src_line_size = w * ((bits_per_pixel + 7) / 8);
        uint8_t* dst  = (uint8_t*)fb->pixels + y * fb->pitch + x *
                        fb->bytes_per_pixel;
        for (n = 0; n < h; n++) {
            memcpy(dst, src, src_line_size);
            src += src_line_size;
            dst += fb->pitch;
        }
        qframebuffer_update(fb, x, y, w, h);
    }
    free(pixels);
}

static void
_fbUpdatesImpl_io_callback(void* opaque, int fd, unsigned events)
{
    FrameBufferImpl* fbi = opaque;
    int  ret;

    
    for (;;) {
        
        ret = socket_recv(fbi->sock, fbi->reader_buffer + fbi->reader_offset,
                          fbi->reader_bytes - fbi->reader_offset);
        if (ret == 0) {
            
            fbUpdatesImpl_destroy();
            return;
        }
        if (ret < 0) {
            if (errno == EINTR) {
                
                continue;
            } else if (errno == EWOULDBLOCK || errno == EAGAIN) {
                
                return;
            }
        }

        fbi->reader_offset += ret;
        if (fbi->reader_offset != fbi->reader_bytes) {
            
            continue;
        }

        
        if (fbi->fb_state == EXPECTS_HEADER) {
            
            fbi->fb_state = EXPECTS_PIXELS;
            fbi->reader_offset = 0;
            fbi->reader_bytes = fbi->update_header.w *
                                      fbi->update_header.h *
                                      (fbi->bits_per_pixel / 8);
            fbi->reader_buffer = malloc(fbi->reader_bytes);
            if (fbi->reader_buffer == NULL) {
                APANIC("Unable to allocate memory for framebuffer update\n");
            }
        } else {
            
             uint8_t* pixels = fbi->reader_buffer;

            fbi->fb_state = EXPECTS_HEADER;
            fbi->reader_offset = 0;
            fbi->reader_bytes = sizeof(FBUpdateMessage);
            fbi->reader_buffer = (uint8_t*)&fbi->update_header;

            
            _update_rect(fbi->fb, fbi->update_header.x,
                        fbi->update_header.y, fbi->update_header.w,
                        fbi->update_header.h, fbi->bits_per_pixel,
                        pixels);
        }
    }
}

int
fbUpdatesImpl_create(SockAddress* console_socket,
              const char* protocol,
              QFrameBuffer* fb,
              Looper* looper)
{
    FrameBufferImpl* fbi = &_fbImpl;
    char* handshake = NULL;
    char switch_cmd[256];

    
    fbi->fb = fb;
    fbi->reader_buffer = (uint8_t*)&fbi->update_header;
    fbi->reader_offset = 0;
    fbi->reader_bytes = sizeof(FBUpdateMessage);

    
    snprintf(switch_cmd, sizeof(switch_cmd), "framebuffer %s", protocol);
    fbi->core_connection =
        core_connection_create_and_switch(console_socket, switch_cmd, &handshake);
    if (fbi->core_connection == NULL) {
        derror("Unable to connect to the framebuffer service: %s\n",
               errno_str);
        return -1;
    }

    
    
    fbi->bits_per_pixel = 0;
    if (handshake != NULL) {
        char* bpp = strstr(handshake, "bitsperpixel=");
        if (bpp != NULL) {
            char* end;
            bpp += strlen("bitsperpixel=");
            end = strchr(bpp, ' ');
            if (end == NULL) {
                end = bpp + strlen(bpp);
            }
            fbi->bits_per_pixel = strtol(bpp, &end, 0);
        }
    }
    if (!fbi->bits_per_pixel) {
        derror("Unexpected core framebuffer reply: %s\n"
               "Bits per pixel property is not there, or is invalid\n",
               handshake);
        fbUpdatesImpl_destroy();
        return -1;
    }

    fbi->sock = core_connection_get_socket(fbi->core_connection);

    
    loopIo_init(fbi->io, looper, fbi->sock,
                _fbUpdatesImpl_io_callback, &_fbImpl);
    loopIo_wantRead(fbi->io);
    {
        
        
        FBRequestHeader hd;
        SyncSocket* sk = syncsocket_init(fbi->sock);

        hd.request_type = AFB_REQUEST_REFRESH;
        syncsocket_start_write(sk);
        syncsocket_write(sk, &hd, sizeof(hd), 5000);
        syncsocket_stop_write(sk);
        syncsocket_free(sk);
    }

    fprintf(stdout, "framebuffer is now connected to the core at %s.",
            sock_address_to_string(console_socket));
    if (handshake != NULL) {
        if (handshake[0] != '\0') {
            fprintf(stdout, " Handshake: %s", handshake);
        }
        free(handshake);
    }
    fprintf(stdout, "\n");

    return 0;
}

void
fbUpdatesImpl_destroy(void)
{
    FrameBufferImpl* fbi = &_fbImpl;

    if (fbi->core_connection != NULL) {
        
        loopIo_done(fbi->io);

        
        core_connection_close(fbi->core_connection);
        core_connection_free(fbi->core_connection);
        fbi->core_connection = NULL;
    }

    fbi->fb = NULL;
    if (fbi->reader_buffer != NULL &&
        fbi->reader_buffer != (uint8_t*)&fbi->update_header) {
        free(fbi->reader_buffer);
        fbi->reader_buffer = (uint8_t*)&fbi->update_header;
    }
}
