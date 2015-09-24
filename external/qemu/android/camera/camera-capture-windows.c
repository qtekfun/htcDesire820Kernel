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


#include <windows.h>
#include <vfw.h>
#include "android/camera/camera-capture.h"
#include "android/camera/camera-format-converters.h"

#define  E(...)    derror(__VA_ARGS__)
#define  W(...)    dwarning(__VA_ARGS__)
#define  D(...)    VERBOSE_PRINT(camera,__VA_ARGS__)
#define  D_ACTIVE  VERBOSE_CHECK(camera)

#define  T_ACTIVE   0

#if T_ACTIVE
#define  T(...)    VERBOSE_PRINT(camera,__VA_ARGS__)
#else
#define  T(...)    ((void)0)
#endif

static const char* _default_window_name = "AndroidEmulatorVC";

typedef struct WndCameraDevice WndCameraDevice;
struct WndCameraDevice {
    
    CameraDevice        header;
    
    char*               window_name;
    
    int                 input_channel;


    
    HWND                cap_window;
    HDC                 dc;
    
    BITMAPINFO*         frame_bitmap;
    BITMAPINFO*         gdi_bitmap;
    
    uint8_t*            framebuffer;
    
    size_t              framebuffer_size;
    
    uint32_t            pixel_format;
    
    int                 is_top_down;
    int                 use_clipboard;
    
    void*               last_frame;
    
    uint32_t            last_frame_size;
};


static WndCameraDevice*
_camera_device_alloc(void)
{
    WndCameraDevice* cd = (WndCameraDevice*)malloc(sizeof(WndCameraDevice));
    if (cd != NULL) {
        memset(cd, 0, sizeof(WndCameraDevice));
        cd->header.opaque = cd;
    } else {
        E("%s: Unable to allocate WndCameraDevice instance", __FUNCTION__);
    }
    return cd;
}

static void
_camera_device_free(WndCameraDevice* cd)
{
    if (cd != NULL) {
        if (cd->cap_window != NULL) {
            
            capDriverDisconnect(cd->cap_window);

            if (cd->dc != NULL) {
                W("%s: Frames should not be capturing at this point",
                  __FUNCTION__);
                ReleaseDC(cd->cap_window, cd->dc);
                cd->dc = NULL;
            }
            
            DestroyWindow(cd->cap_window);
            cd->cap_window = NULL;
        }
        if (cd->gdi_bitmap != NULL) {
            free(cd->gdi_bitmap);
        }
        if (cd->frame_bitmap != NULL) {
            free(cd->frame_bitmap);
        }
        if (cd->window_name != NULL) {
            free(cd->window_name);
        }
        if (cd->framebuffer != NULL) {
            free(cd->framebuffer);
        }
        if (cd->last_frame != NULL) {
            free(cd->last_frame);
        }
        AFREE(cd);
    } else {
        W("%s: No descriptor", __FUNCTION__);
    }
}

static void
_camera_device_reset(WndCameraDevice* cd)
{
    if (cd != NULL && cd->cap_window != NULL) {
        capDriverDisconnect(cd->cap_window);
        if (cd->dc != NULL) {
            ReleaseDC(cd->cap_window, cd->dc);
            cd->dc = NULL;
        }
        if (cd->gdi_bitmap != NULL) {
            free(cd->gdi_bitmap);
            cd->gdi_bitmap = NULL;
        }
        if (cd->frame_bitmap != NULL) {
            free(cd->frame_bitmap);
            cd->frame_bitmap = NULL;
        }
        if (cd->framebuffer != NULL) {
            free(cd->framebuffer);
            cd->framebuffer = NULL;
        }
        if (cd->last_frame != NULL) {
            free(cd->last_frame);
            cd->last_frame = NULL;
        }
        cd->last_frame_size = 0;

        
        DestroyWindow(cd->cap_window);
        cd->cap_window = capCreateCaptureWindow(cd->window_name, WS_CHILD, 0, 0,
                                                0, 0, HWND_MESSAGE, 1);
        if (cd->cap_window != NULL) {
            
            capSetUserData(cd->cap_window, cd);
        }
    }
}

static __inline__ int
_abs(int val)
{
    return (val < 0) ? -val : val;
}

static LRESULT CALLBACK
_on_captured_frame(HWND hwnd, LPVIDEOHDR hdr)
{
    
    WndCameraDevice* wcd = (WndCameraDevice*)capGetUserData(hwnd);

    
    if (wcd->last_frame_size < hdr->dwBytesUsed) {
        wcd->last_frame_size = hdr->dwBytesUsed;
        if (wcd->last_frame != NULL) {
            free(wcd->last_frame);
        }
        wcd->last_frame = malloc(wcd->last_frame_size);
    }

    
    memcpy(wcd->last_frame, hdr->lpData, hdr->dwBytesUsed);

    if (wcd->frame_bitmap->bmiHeader.biCompression == BI_RGB) {
        if (wcd->frame_bitmap->bmiHeader.biBitCount == 32) {
            wcd->pixel_format = V4L2_PIX_FMT_BGR32;
        } else if (wcd->frame_bitmap->bmiHeader.biBitCount == 16) {
            wcd->pixel_format = V4L2_PIX_FMT_RGB565;
        } else {
            wcd->pixel_format = V4L2_PIX_FMT_BGR24;
        }
    } else {
        wcd->pixel_format = wcd->frame_bitmap->bmiHeader.biCompression;
    }

    return (LRESULT)0;
}


CameraDevice*
camera_device_open(const char* name, int inp_channel)
{
    WndCameraDevice* wcd;

    
    wcd = _camera_device_alloc();
    if (wcd == NULL) {
        E("%s: Unable to allocate WndCameraDevice instance", __FUNCTION__);
        return NULL;
    }
    wcd->window_name = (name != NULL) ? ASTRDUP(name) :
                                        ASTRDUP(_default_window_name);
    if (wcd->window_name == NULL) {
        E("%s: Unable to save window name", __FUNCTION__);
        _camera_device_free(wcd);
        return NULL;
    }
    wcd->input_channel = inp_channel;

    wcd->cap_window = capCreateCaptureWindow(wcd->window_name, WS_CHILD, 0, 0,
                                             0, 0, HWND_MESSAGE, 1);
    if (wcd->cap_window == NULL) {
        E("%s: Unable to create video capturing window '%s': %d",
          __FUNCTION__, wcd->window_name, GetLastError());
        _camera_device_free(wcd);
        return NULL;
    }
    
    capSetUserData(wcd->cap_window, wcd);

    return &wcd->header;
}

int
camera_device_start_capturing(CameraDevice* cd,
                              uint32_t pixel_format,
                              int frame_width,
                              int frame_height)
{
    WndCameraDevice* wcd;
    HBITMAP bm_handle;
    BITMAP  bitmap;
    size_t format_info_size;
    CAPTUREPARMS cap_param;

    if (cd == NULL || cd->opaque == NULL) {
        E("%s: Invalid camera device descriptor", __FUNCTION__);
        return -1;
    }
    wcd = (WndCameraDevice*)cd->opaque;

    
    if (wcd->dc != NULL) {
        W("%s: Capturing is already on on device '%s'",
          __FUNCTION__, wcd->window_name);
        return 0;
    }

    
    if (!capDriverConnect(wcd->cap_window, wcd->input_channel)) {
        return -1;
    }

    
    format_info_size = capGetVideoFormatSize(wcd->cap_window);
    if (format_info_size == 0) {
        E("%s: Unable to get video format size: %d",
          __FUNCTION__, GetLastError());
        _camera_device_reset(wcd);
        return -1;
    }
    wcd->frame_bitmap = (BITMAPINFO*)malloc(format_info_size);
    if (wcd->frame_bitmap == NULL) {
        E("%s: Unable to allocate frame bitmap info buffer", __FUNCTION__);
        _camera_device_reset(wcd);
        return -1;
    }
    if (!capGetVideoFormat(wcd->cap_window, wcd->frame_bitmap,
                           format_info_size)) {
        E("%s: Unable to obtain video format: %d", __FUNCTION__, GetLastError());
        _camera_device_reset(wcd);
        return -1;
    }

    
    if (wcd->frame_bitmap->bmiHeader.biWidth != frame_width ||
            abs(wcd->frame_bitmap->bmiHeader.biHeight) != frame_height) {
        
        wcd->frame_bitmap->bmiHeader.biWidth = frame_width;
        wcd->frame_bitmap->bmiHeader.biHeight = frame_height;
        if (wcd->frame_bitmap->bmiHeader.biBitCount == 24) {
            
            uint32_t bpl = (frame_width * 3 + 1) & ~1;
            wcd->frame_bitmap->bmiHeader.biSizeImage = bpl * frame_height;
        } else {
            wcd->frame_bitmap->bmiHeader.biSizeImage =
                (frame_width * frame_height * wcd->frame_bitmap->bmiHeader.biBitCount) / 8;
        }
        if (!capSetVideoFormat(wcd->cap_window, wcd->frame_bitmap,
                               format_info_size)) {
            E("%s: Unable to set video format: %d", __FUNCTION__, GetLastError());
            _camera_device_reset(wcd);
            return -1;
        }
    }

    if (wcd->frame_bitmap->bmiHeader.biCompression > BI_PNG) {
        D("%s: Video capturing driver has reported pixel format %.4s",
          __FUNCTION__, (const char*)&wcd->frame_bitmap->bmiHeader.biCompression);
    }

    if (wcd->frame_bitmap->bmiHeader.biHeight < 0) {
        wcd->frame_bitmap->bmiHeader.biHeight =
            -wcd->frame_bitmap->bmiHeader.biHeight;
        wcd->is_top_down = 1;
    } else {
        wcd->is_top_down = 0;
    }

    wcd->dc = GetDC(wcd->cap_window);
    if (wcd->dc == NULL) {
        E("%s: Unable to obtain DC for %s: %d",
          __FUNCTION__, wcd->window_name, GetLastError());
        _camera_device_reset(wcd);
        return -1;
    }

    
    if (capCaptureGetSetup(wcd->cap_window, &cap_param, sizeof(cap_param))) {
        
        cap_param.fYield = TRUE;
        
        cap_param.fMakeUserHitOKToCapture = FALSE;
        capCaptureSetSetup(wcd->cap_window, &cap_param, sizeof(cap_param));
    }


    if (!capGrabFrameNoStop(wcd->cap_window) ||
        !capEditCopy(wcd->cap_window) ||
        !OpenClipboard(wcd->cap_window)) {
        E("%s: Device '%s' is unable to save frame to the clipboard: %d",
          __FUNCTION__, wcd->window_name, GetLastError());
        _camera_device_reset(wcd);
        return -1;
    }

    bm_handle = (HBITMAP)GetClipboardData(CF_BITMAP);
    if (bm_handle == NULL) {
        E("%s: Device '%s' is unable to obtain frame from the clipboard: %d",
          __FUNCTION__, wcd->window_name, GetLastError());
        CloseClipboard();
        _camera_device_reset(wcd);
        return -1;
    }

    
    if (!GetObject(bm_handle, sizeof(BITMAP), &bitmap)) {
        E("%s: Device '%s' is unable to obtain frame's bitmap: %d",
          __FUNCTION__, wcd->window_name, GetLastError());
        EmptyClipboard();
        CloseClipboard();
        _camera_device_reset(wcd);
        return -1;
    }

    
    EmptyClipboard();
    CloseClipboard();

    
    if (wcd->frame_bitmap->bmiHeader.biWidth != bitmap.bmWidth ||
        wcd->frame_bitmap->bmiHeader.biHeight != bitmap.bmHeight ) {
        E("%s: Requested dimensions %dx%d do not match the actual %dx%d",
          __FUNCTION__, frame_width, frame_height,
          wcd->frame_bitmap->bmiHeader.biWidth,
          wcd->frame_bitmap->bmiHeader.biHeight);
        _camera_device_reset(wcd);
        return -1;
    }

    
    wcd->gdi_bitmap = (BITMAPINFO*)malloc(wcd->frame_bitmap->bmiHeader.biSize);
    if (wcd->gdi_bitmap == NULL) {
        E("%s: Unable to allocate gdi bitmap info", __FUNCTION__);
        _camera_device_reset(wcd);
        return -1;
    }
    memcpy(wcd->gdi_bitmap, wcd->frame_bitmap,
           wcd->frame_bitmap->bmiHeader.biSize);
    wcd->gdi_bitmap->bmiHeader.biCompression = BI_RGB;
    wcd->gdi_bitmap->bmiHeader.biBitCount = bitmap.bmBitsPixel;
    wcd->gdi_bitmap->bmiHeader.biSizeImage = bitmap.bmWidthBytes * bitmap.bmWidth;
    if (wcd->is_top_down) {
        wcd->gdi_bitmap->bmiHeader.biHeight =
            wcd->frame_bitmap->bmiHeader.biHeight;
    } else {
        wcd->gdi_bitmap->bmiHeader.biHeight =
            -wcd->frame_bitmap->bmiHeader.biHeight;
    }

    
    wcd->framebuffer = (uint8_t*)malloc(wcd->gdi_bitmap->bmiHeader.biSizeImage);
    if (wcd->framebuffer == NULL) {
        E("%s: Unable to allocate %d bytes for framebuffer",
          __FUNCTION__, wcd->gdi_bitmap->bmiHeader.biSizeImage);
        _camera_device_reset(wcd);
        return -1;
    }

    
    if (wcd->gdi_bitmap->bmiHeader.biBitCount == 16) {
        wcd->pixel_format = V4L2_PIX_FMT_RGB565;
    } else if (wcd->gdi_bitmap->bmiHeader.biBitCount == 24) {
        wcd->pixel_format = V4L2_PIX_FMT_BGR24;
    } else if (wcd->gdi_bitmap->bmiHeader.biBitCount == 32) {
        wcd->pixel_format = V4L2_PIX_FMT_BGR32;
    } else {
        E("%s: Unsupported number of bits per pixel %d",
          __FUNCTION__, wcd->gdi_bitmap->bmiHeader.biBitCount);
        _camera_device_reset(wcd);
        return -1;
    }

    D("%s: Capturing device '%s': %d bits per pixel in %.4s [%dx%d] frame",
      __FUNCTION__, wcd->window_name, wcd->gdi_bitmap->bmiHeader.biBitCount,
      (const char*)&wcd->pixel_format, wcd->frame_bitmap->bmiHeader.biWidth,
      wcd->frame_bitmap->bmiHeader.biHeight);

    
    wcd->use_clipboard = 1;
    if (capSetCallbackOnFrame(wcd->cap_window, _on_captured_frame)) {
        
        wcd->use_clipboard = 0;
    }

    return 0;
}

int
camera_device_stop_capturing(CameraDevice* cd)
{
    WndCameraDevice* wcd;
    if (cd == NULL || cd->opaque == NULL) {
        E("%s: Invalid camera device descriptor", __FUNCTION__);
        return -1;
    }
    wcd = (WndCameraDevice*)cd->opaque;

    
    capSetCallbackOnFrame(wcd->cap_window, NULL);

    
    if (wcd->dc == NULL) {
        W("%s: Device '%s' is not capturing video",
          __FUNCTION__, wcd->window_name);
        return 0;
    }
    ReleaseDC(wcd->cap_window, wcd->dc);
    wcd->dc = NULL;

    
    _camera_device_reset(wcd);

    return 0;
}

static int
_camera_device_read_frame_callback(WndCameraDevice* wcd,
                                   ClientFrameBuffer* framebuffers,
                                   int fbs_num,
                                   float r_scale,
                                   float g_scale,
                                   float b_scale,
                                   float exp_comp)
{
    if (!capGrabFrameNoStop(wcd->cap_window) || wcd->last_frame == NULL) {
        E("%s: Device '%s' is unable to grab a frame: %d",
          __FUNCTION__, wcd->window_name, GetLastError());
        return -1;
    }

    
    return convert_frame(wcd->last_frame,
                         wcd->pixel_format,
                         wcd->frame_bitmap->bmiHeader.biSizeImage,
                         wcd->frame_bitmap->bmiHeader.biWidth,
                         wcd->frame_bitmap->bmiHeader.biHeight,
                         framebuffers, fbs_num,
                         r_scale, g_scale, b_scale, exp_comp);
}

static int
_camera_device_read_frame_clipboard(WndCameraDevice* wcd,
                                    ClientFrameBuffer* framebuffers,
                                    int fbs_num,
                                    float r_scale,
                                    float g_scale,
                                    float b_scale,
                                    float exp_comp)
{
    HBITMAP bm_handle;

    if (!capGrabFrameNoStop(wcd->cap_window) ||
        !capEditCopy(wcd->cap_window) ||
        !OpenClipboard(wcd->cap_window)) {
        E("%s: Device '%s' is unable to save frame to the clipboard: %d",
          __FUNCTION__, wcd->window_name, GetLastError());
        return -1;
    }

    bm_handle = (HBITMAP)GetClipboardData(CF_BITMAP);
    if (bm_handle == NULL) {
        E("%s: Device '%s' is unable to obtain frame from the clipboard: %d",
          __FUNCTION__, wcd->window_name, GetLastError());
        EmptyClipboard();
        CloseClipboard();
        return -1;
    }

    
    if (wcd->gdi_bitmap->bmiHeader.biHeight > 0) {
        wcd->gdi_bitmap->bmiHeader.biHeight = -wcd->gdi_bitmap->bmiHeader.biHeight;
    }

    if (!GetDIBits(wcd->dc, bm_handle, 0, wcd->frame_bitmap->bmiHeader.biHeight,
                   wcd->framebuffer, wcd->gdi_bitmap, DIB_RGB_COLORS)) {
        E("%s: Device '%s' is unable to transfer frame to the framebuffer: %d",
          __FUNCTION__, wcd->window_name, GetLastError());
        EmptyClipboard();
        CloseClipboard();
        return -1;
    }

    if (wcd->gdi_bitmap->bmiHeader.biHeight < 0) {
        wcd->gdi_bitmap->bmiHeader.biHeight = -wcd->gdi_bitmap->bmiHeader.biHeight;
    }

    EmptyClipboard();
    CloseClipboard();

    
    return convert_frame(wcd->framebuffer,
                         wcd->pixel_format,
                         wcd->gdi_bitmap->bmiHeader.biSizeImage,
                         wcd->frame_bitmap->bmiHeader.biWidth,
                         wcd->frame_bitmap->bmiHeader.biHeight,
                         framebuffers, fbs_num,
                         r_scale, g_scale, b_scale, exp_comp);
}

int
camera_device_read_frame(CameraDevice* cd,
                         ClientFrameBuffer* framebuffers,
                         int fbs_num,
                         float r_scale,
                         float g_scale,
                         float b_scale,
                         float exp_comp)
{
    WndCameraDevice* wcd;

    
    if (cd == NULL || cd->opaque == NULL) {
        E("%s: Invalid camera device descriptor", __FUNCTION__);
        return -1;
    }
    wcd = (WndCameraDevice*)cd->opaque;
    if (wcd->dc == NULL) {
        W("%s: Device '%s' is not captuing video",
          __FUNCTION__, wcd->window_name);
        return -1;
    }

    return wcd->use_clipboard ?
        _camera_device_read_frame_clipboard(wcd, framebuffers, fbs_num, r_scale,
                                            g_scale, b_scale, exp_comp) :
        _camera_device_read_frame_callback(wcd, framebuffers, fbs_num, r_scale,
                                           g_scale, b_scale, exp_comp);
}

void
camera_device_close(CameraDevice* cd)
{
    
    if (cd == NULL || cd->opaque == NULL) {
        E("%s: Invalid camera device descriptor", __FUNCTION__);
    } else {
        WndCameraDevice* wcd = (WndCameraDevice*)cd->opaque;
        _camera_device_free(wcd);
    }
}

int
enumerate_camera_devices(CameraInfo* cis, int max)
{
static const CameraFrameDim _emulate_dims[] =
{
  
  {640, 480},
  
  {352, 288},
  
  {320, 240},
  
  {176, 144}
};
    int inp_channel, found = 0;

    for (inp_channel = 0; inp_channel < 10 && found < max; inp_channel++) {
        char name[256];
        CameraDevice* cd;

        snprintf(name, sizeof(name), "%s%d", _default_window_name, found);
        cd = camera_device_open(name, inp_channel);
        if (cd != NULL) {
            WndCameraDevice* wcd = (WndCameraDevice*)cd->opaque;

            if (!camera_device_start_capturing(cd, V4L2_PIX_FMT_RGB32, 640, 480)) {
                cis[found].frame_sizes = (CameraFrameDim*)malloc(sizeof(_emulate_dims));
                if (cis[found].frame_sizes != NULL) {
                    char disp_name[24];
                    sprintf(disp_name, "webcam%d", found);
                    cis[found].display_name = ASTRDUP(disp_name);
                    cis[found].device_name = ASTRDUP(name);
                    cis[found].direction = ASTRDUP("front");
                    cis[found].inp_channel = inp_channel;
                    cis[found].frame_sizes_num = sizeof(_emulate_dims) / sizeof(*_emulate_dims);
                    memcpy(cis[found].frame_sizes, _emulate_dims, sizeof(_emulate_dims));
                    cis[found].pixel_format = wcd->pixel_format;
                    cis[found].in_use = 0;
                    found++;
                } else {
                    E("%s: Unable to allocate dimensions", __FUNCTION__);
                }
                camera_device_stop_capturing(cd);
            } else {
                
                camera_device_close(cd);
                break;
            }
            camera_device_close(cd);
        } else {
            
            break;
        }
    }

    return found;
}
