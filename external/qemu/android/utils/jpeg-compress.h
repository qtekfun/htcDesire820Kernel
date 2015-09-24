/* Copyright (C) 2011 The Android Open Source Project
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
#ifndef _ANDROID_UTILS_JPEG_COMPRESS_H
#define _ANDROID_UTILS_JPEG_COMPRESS_H



typedef struct AJPEGDesc AJPEGDesc;

extern AJPEGDesc* jpeg_compressor_create(int header_size, int chunk_size);

extern void jpeg_compressor_destroy(AJPEGDesc* dsc);

extern int jpeg_compressor_get_jpeg_size(const AJPEGDesc* dsc);

extern void* jpeg_compressor_get_buffer(const AJPEGDesc* dsc);

extern int jpeg_compressor_get_header_size(const AJPEGDesc* dsc);

extern void jpeg_compressor_compress_fb(AJPEGDesc* dsc,
                                        int x, int y, int w, int h,
                                        int num_lines,
                                        int bpp, int bpl,
                                        const uint8_t* fb,
                                        int jpeg_quality,
                                        int ydir);

#endif  
