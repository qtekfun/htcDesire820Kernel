/*
 * va_drmcommon.h - Common utilities for DRM-based drivers
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

#ifndef VA_DRM_COMMON_H
#define VA_DRM_COMMON_H

enum {
    
    VA_DRM_AUTH_NONE    = 0,
    /**
     * \brief Connected. Authenticated with DRI1 protocol.
     *
     * @deprecated
     * This is a deprecated authentication type. All DRI-based drivers have
     * been migrated to use the DRI2 protocol. Newly written drivers shall
     * use DRI2 protocol only, or a custom authentication means. e.g. opt
     * for authenticating on the VA driver side, instead of libva side.
     */
    VA_DRM_AUTH_DRI1    = 1,
    VA_DRM_AUTH_DRI2    = 2,
    VA_DRM_AUTH_CUSTOM  = 3
};

struct drm_state {
    
    int         fd;
    
    int         auth_type;
};

#define VA_SURFACE_ATTRIB_MEM_TYPE_KERNEL_DRM		0x10000000
#define VA_SURFACE_ATTRIB_MEM_TYPE_DRM_PRIME		0x20000000

#endif 
