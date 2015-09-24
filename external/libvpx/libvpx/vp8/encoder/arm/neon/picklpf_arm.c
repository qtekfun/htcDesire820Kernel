/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "vp8/common/loopfilter.h"
#include "vpx_scale/yv12config.h"

extern void vp8_memcpy_partial_neon(unsigned char *dst_ptr,
                                    unsigned char *src_ptr,
                                    int sz);


void vp8_yv12_copy_partial_frame_neon(YV12_BUFFER_CONFIG *src_ybc,
                                      YV12_BUFFER_CONFIG *dst_ybc)
{
    unsigned char *src_y, *dst_y;
    int yheight;
    int ystride;
    int yoffset;
    int linestocopy;

    yheight  = src_ybc->y_height;
    ystride  = src_ybc->y_stride;

    
    linestocopy = (yheight >> 4) / PARTIAL_FRAME_FRACTION;
    linestocopy = linestocopy ? linestocopy << 4 : 16;     

    linestocopy += 4;
    
    yoffset  = ystride * (((yheight >> 5) * 16) - 4);
    src_y = src_ybc->y_buffer + yoffset;
    dst_y = dst_ybc->y_buffer + yoffset;

    vp8_memcpy_partial_neon(dst_y, src_y, ystride * linestocopy);
}