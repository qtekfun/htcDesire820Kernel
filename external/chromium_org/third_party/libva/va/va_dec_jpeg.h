/*
 * Copyright (c) 2007-2012 Intel Corporation. All Rights Reserved.
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


#ifndef VA_DEC_JPEG_H
#define VA_DEC_JPEG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <va/va.h>


typedef struct _VAPictureParameterBufferJPEGBaseline {
    
    unsigned short      picture_width;
    
    unsigned short      picture_height;

    struct {
        
        unsigned char   component_id;
        
        unsigned char   h_sampling_factor;
        
        unsigned char   v_sampling_factor;
        
        unsigned char   quantiser_table_selector;
    }                   components[255];
    
    unsigned char       num_components;
} VAPictureParameterBufferJPEGBaseline;

typedef struct _VAIQMatrixBufferJPEGBaseline {
    
    unsigned char       load_quantiser_table[4];
    
    unsigned char       quantiser_table[4][64];
} VAIQMatrixBufferJPEGBaseline;

typedef struct _VAHuffmanTableBufferJPEGBaseline {
    
    unsigned char       load_huffman_table[2];
    
    struct {
        
        
        
        unsigned char   num_dc_codes[16];
        
        unsigned char   dc_values[12];
        
        
        
        
        unsigned char   num_ac_codes[16];
        
        unsigned char   ac_values[162];
        
        unsigned char   pad[2];
        
    }                   huffman_table[2];
} VAHuffmanTableBufferJPEGBaseline;

typedef struct _VASliceParameterBufferJPEGBaseline {
    
    
    
    unsigned int        slice_data_size;
    
    unsigned int        slice_data_offset;
    
    unsigned int        slice_data_flag;
    

    
    unsigned int        slice_horizontal_position;
    
    unsigned int        slice_vertical_position;

    struct {
        
        unsigned char   component_selector;
        
        unsigned char   dc_table_selector;
        
        unsigned char   ac_table_selector;
    }                   components[4];
    
    unsigned char       num_components;

    
    unsigned short      restart_interval;
    
    unsigned int        num_mcus;
} VASliceParameterBufferJPEGBaseline;


#ifdef __cplusplus
}
#endif

#endif 
