/*
 * Copyright (c) 2007-2009 Intel Corporation. All Rights Reserved.
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
#ifndef _VA_TPI_H_
#define _VA_TPI_H_

#include <va/va.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    VAExternalMemoryNULL, 
    VAExternalMemoryV4L2Buffer,
    VAExternalMemoryCIFrame, 
    VAExternalMemoryUserPointer, 
    VAExternalMemoryKernelDRMBufffer, 
    VAExternalMemoryAndroidGrallocBuffer, 
} VASurfaceMemoryType;

typedef struct _VASurfaceAttributeTPI {
    VASurfaceMemoryType type;
    unsigned int width;
    unsigned int height;
    unsigned int size;
    unsigned int pixel_format; 
    unsigned int tiling; 
    unsigned int luma_stride; 
    unsigned int chroma_u_stride; 
    unsigned int chroma_v_stride;
    unsigned int luma_offset; 
    unsigned int chroma_u_offset; 
    unsigned int chroma_v_offset; 
    unsigned int count; 
    unsigned int *buffers; 
    unsigned int reserved[4]; 
} VASurfaceAttributeTPI;


VAStatus vaPutSurfaceBuf (
    VADisplay dpy,
    VASurfaceID surface,
    unsigned char* data,
    int* data_len,
    short srcx,
    short srcy,
    unsigned short srcw,
    unsigned short srch,
    short destx,
    short desty,
    unsigned short destw,
    unsigned short desth,
    VARectangle *cliprects, 
    unsigned int number_cliprects, 
    unsigned int flags 
);


VAStatus vaCreateSurfacesWithAttribute (
    VADisplay dpy,
    int width,
    int height,
    int format,
    int num_surfaces,
    VASurfaceID *surfaces,       
    VASurfaceAttributeTPI *attribute_tpi
);

#ifdef __cplusplus
}
#endif

#endif 
