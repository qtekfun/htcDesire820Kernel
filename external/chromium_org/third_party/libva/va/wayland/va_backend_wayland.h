/*
 * va_backend_wayland.h - VA driver implementation hooks for Wayland
 *
 * Copyright (c) 2012 Intel Corporation. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL INTEL AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef VA_BACKEND_WAYLAND_H
#define VA_BACKEND_WAYLAND_H

#include <va/va.h>
#include <wayland-client.h>

#define VA_WAYLAND_API_VERSION  (0x574c4400) 

struct VADriverContext;

struct VADriverVTableWayland {
    unsigned int version;

    
    VAStatus (*vaGetSurfaceBufferWl)(
        struct VADriverContext *ctx,
        VASurfaceID             surface,
        unsigned int            flags,
        struct wl_buffer      **out_buffer
    );

    
    VAStatus (*vaGetImageBufferWl)(
        struct VADriverContext *ctx,
        VAImageID               image,
        unsigned int            flags,
        struct wl_buffer      **out_buffer
    );
};

#endif 
