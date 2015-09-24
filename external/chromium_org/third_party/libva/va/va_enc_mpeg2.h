/*
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


#ifndef _VA_ENC_MPEG2_H_
#define _VA_ENC_MPEG2_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef VAIQMatrixBufferMPEG2 VAQMatrixBufferMPEG2;

typedef enum {
    VAEncPackedHeaderMPEG2_SPS = VAEncPackedHeaderSequence,
    VAEncPackedHeaderMPEG2_PPS = VAEncPackedHeaderPicture,
    VAEncPackedHeaderMPEG2_Slice = VAEncPackedHeaderSlice,
} VAEncPackedHeaderTypeMPEG2;

typedef struct _VAEncSequenceParameterBufferMPEG2 {
    
    unsigned int intra_period;
    
    unsigned int ip_period;
    unsigned short picture_width;
    unsigned short picture_height;
    unsigned int bits_per_second;
    float frame_rate;
    
    unsigned short aspect_ratio_information;
    
    unsigned int vbv_buffer_size;

    union {
        struct {
            
            unsigned int profile_and_level_indication   : 8;
            
            unsigned int progressive_sequence           : 1;
            
            unsigned int chroma_format                  : 2;
            
            unsigned int low_delay                      : 1;
            
            unsigned int frame_rate_extension_n         : 2;
            
            unsigned int frame_rate_extension_d         : 5;
        } bits;
        unsigned int value;
    } sequence_extension;

    
    unsigned int new_gop_header;

    union {
        struct {
            
            unsigned int time_code                      : 25;
            
            unsigned int closed_gop                     : 1;
            
            unsigned int broken_link                    : 1;
        } bits;
        unsigned int value;
    } gop_header;
} VAEncSequenceParameterBufferMPEG2;

typedef struct _VAEncPictureParameterBufferMPEG2 {
    
    VASurfaceID forward_reference_picture;
    
    VASurfaceID backward_reference_picture;
    
    VASurfaceID reconstructed_picture;
    VABufferID coded_buf;
    unsigned char last_picture;
    
    VAEncPictureType picture_type;
    
    unsigned int temporal_reference;
    
    unsigned int vbv_delay;
    
    unsigned char f_code[2][2];
    union {
        struct {
            
            unsigned int intra_dc_precision             : 2; 
            
            unsigned int picture_structure              : 2; 
            
            unsigned int top_field_first                : 1; 
            
            unsigned int frame_pred_frame_dct           : 1; 
            
            unsigned int concealment_motion_vectors     : 1;
            
            unsigned int q_scale_type                   : 1;
            
            unsigned int intra_vlc_format               : 1;
            
            unsigned int alternate_scan                 : 1;
            
            unsigned int repeat_first_field             : 1;
            
            unsigned int progressive_frame              : 1;
            
            unsigned int composite_display_flag         : 1;
        } bits;
        unsigned int value;
    } picture_coding_extension;

    union {
        struct {
                        
            unsigned int v_axis                         : 1;
            
            unsigned int field_sequence                 : 3;
            
            unsigned int sub_carrier                    : 1;
            
            unsigned int burst_amplitude                : 7;
            
            unsigned int sub_carrier_phase              : 8;
        } bits;
        unsigned int value;
    } composite_display;
} VAEncPictureParameterBufferMPEG2;

typedef struct _VAEncSliceParameterBufferMPEG2 {
    
    unsigned int macroblock_address;
    
    unsigned int num_macroblocks;
    
    int quantiser_scale_code;
    
    int is_intra_slice;
} VAEncSliceParameterBufferMPEG2;


#ifdef __cplusplus
}
#endif

#endif 
