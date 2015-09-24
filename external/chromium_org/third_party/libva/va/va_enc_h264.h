/*
 * Copyright (c) 2007-2011 Intel Corporation. All Rights Reserved.
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


#ifndef VA_ENC_H264_H
#define VA_ENC_H264_H

#ifdef __cplusplus
extern "C" {
#endif


#define H264_LAST_PICTURE_EOSEQ     0x01
#define H264_LAST_PICTURE_EOSTREAM  0x02

typedef enum {
    VAEncPackedHeaderH264_SPS   = VAEncPackedHeaderSequence,
    VAEncPackedHeaderH264_PPS   = VAEncPackedHeaderPicture,
    VAEncPackedHeaderH264_Slice = VAEncPackedHeaderSlice,
    VAEncPackedHeaderH264_SEI   = (VAEncPackedHeaderMiscMask | 1),
} VAEncPackedHeaderTypeH264;

typedef struct _VAEncSequenceParameterBufferH264 {
    
    unsigned char   seq_parameter_set_id;
    
    unsigned char   level_idc;
    
    unsigned int    intra_period;
    
    unsigned int    intra_idr_period;
    
    unsigned int    ip_period;
    unsigned int    bits_per_second;
    
    unsigned int    max_num_ref_frames;
    
    unsigned short  picture_width_in_mbs;
    
    unsigned short  picture_height_in_mbs;

    union {
        struct {
            
            unsigned int chroma_format_idc                      : 2;
            
            unsigned int frame_mbs_only_flag                    : 1;
            
            unsigned int mb_adaptive_frame_field_flag           : 1;
            
            unsigned int seq_scaling_matrix_present_flag        : 1;
            
            unsigned int direct_8x8_inference_flag              : 1;
            
            unsigned int log2_max_frame_num_minus4              : 4;
            
            unsigned int pic_order_cnt_type                     : 2;
            
            unsigned int log2_max_pic_order_cnt_lsb_minus4      : 4;
            
            unsigned int delta_pic_order_always_zero_flag       : 1;
        } bits;
        unsigned int value;
    } seq_fields;

    
    unsigned char   bit_depth_luma_minus8;
    
    unsigned char   bit_depth_chroma_minus8;

    
    
    
    unsigned char   num_ref_frames_in_pic_order_cnt_cycle;
    
    int             offset_for_non_ref_pic;
    
    int             offset_for_top_to_bottom_field;
    
    int             offset_for_ref_frame[256];
    

    
    
    
    unsigned char   frame_cropping_flag;
    
    unsigned int    frame_crop_left_offset;
    
    unsigned int    frame_crop_right_offset;
    
    unsigned int    frame_crop_top_offset;
    
    unsigned int    frame_crop_bottom_offset;
    

    
    
    
    unsigned char   vui_parameters_present_flag;
    union {
        struct {
            
            unsigned int aspect_ratio_info_present_flag         : 1;
            
            unsigned int timing_info_present_flag               : 1;
            
            unsigned int bitstream_restriction_flag             : 1;
            
            unsigned int log2_max_mv_length_horizontal          : 5;
            
            unsigned int log2_max_mv_length_vertical            : 5;
        } bits;
        unsigned int value;
    } vui_fields;
    
    unsigned char   aspect_ratio_idc;
    
    unsigned int    sar_width;
    
    unsigned int    sar_height;
    
    unsigned int    num_units_in_tick;
    
    unsigned int    time_scale;
    
} VAEncSequenceParameterBufferH264;

typedef struct _VAEncPictureParameterBufferH264 {
    VAPictureH264   CurrPic;
    VAPictureH264   ReferenceFrames[16];
    VABufferID      coded_buf;

    
    unsigned char   pic_parameter_set_id;
    
    unsigned char   seq_parameter_set_id;

    unsigned char   last_picture;

    unsigned short  frame_num;

    
    unsigned char   pic_init_qp;
    unsigned char   num_ref_idx_l0_active_minus1;
    unsigned char   num_ref_idx_l1_active_minus1;

    
    signed char     chroma_qp_index_offset;
    
    signed char     second_chroma_qp_index_offset;

    union {
        struct {
            
            unsigned int idr_pic_flag                           : 1;
            
            unsigned int reference_pic_flag                     : 2;
            
            unsigned int entropy_coding_mode_flag               : 1;
            
            unsigned int weighted_pred_flag                     : 1;
            
            unsigned int weighted_bipred_idc                    : 2;
            
            unsigned int constrained_intra_pred_flag            : 1;
            
            unsigned int transform_8x8_mode_flag                : 1;
            
            unsigned int deblocking_filter_control_present_flag : 1;
            
            unsigned int redundant_pic_cnt_present_flag         : 1;
            
            unsigned int pic_order_present_flag                 : 1;
            
            unsigned int pic_scaling_matrix_present_flag        : 1;
        } bits;
        unsigned int value;
    } pic_fields;
} VAEncPictureParameterBufferH264;

typedef struct _VAEncSliceParameterBufferH264 {
    
    unsigned int    macroblock_address;
    
    unsigned int    num_macroblocks;
    VABufferID      macroblock_info;
    unsigned char   slice_type;
    
    unsigned char   pic_parameter_set_id;
    
    unsigned short  idr_pic_id;

    
    
    
    unsigned short  pic_order_cnt_lsb;
    
    int             delta_pic_order_cnt_bottom;
    
    
    
    
    int             delta_pic_order_cnt[2];
    

    
    
    unsigned char   direct_spatial_mv_pred_flag;
    

    
    
    unsigned char   num_ref_idx_active_override_flag;
    unsigned char   num_ref_idx_l0_active_minus1;
    unsigned char   num_ref_idx_l1_active_minus1;
    
    VAPictureH264   RefPicList0[32];
    
    VAPictureH264   RefPicList1[32];
    

    
    
    
    unsigned char   luma_log2_weight_denom;
    
    unsigned char   chroma_log2_weight_denom;
    
    unsigned char   luma_weight_l0_flag;
    
    signed short    luma_weight_l0[32];
    
    signed short    luma_offset_l0[32];
    
    unsigned char   chroma_weight_l0_flag;
    
    signed short    chroma_weight_l0[32][2];
    
    signed short    chroma_offset_l0[32][2];
    
    unsigned char   luma_weight_l1_flag;
    
    signed short    luma_weight_l1[32];
    
    signed short    luma_offset_l1[32];
    
    unsigned char   chroma_weight_l1_flag;
    
    signed short    chroma_weight_l1[32][2];
    
    signed short    chroma_offset_l1[32][2];
    

    
    unsigned char   cabac_init_idc;
    
    signed char     slice_qp_delta;
    
    
    
    unsigned char   disable_deblocking_filter_idc;
    
    signed char     slice_alpha_c0_offset_div2;
    
    signed char     slice_beta_offset_div2;
    
} VAEncSliceParameterBufferH264;

#define VA_MB_PRED_AVAIL_TOP_LEFT         (1 << 2)
#define VA_MB_PRED_AVAIL_TOP              (1 << 4)
#define VA_MB_PRED_AVAIL_TOP_RIGHT        (1 << 3)
#define VA_MB_PRED_AVAIL_LEFT             (1 << 6)

/**
 * \brief Macroblock parameter for H.264 encoding in baseline, main & high 
 * profiles.
 *
 * This structure holds per-macroblock information. The buffer must be
 * allocated with as many elements (macroblocks) as necessary to fit
 * the slice to be encoded. Besides, the per-macroblock records must
 * be written in a strict raster order and with no gap. i.e. every
 * macroblock, regardless of its type, shall have an entry.
 */
typedef struct _VAEncMacroblockParameterBufferH264 {
    unsigned char   qp;

    union {
        
        
        struct {
            union {
                unsigned int    pred_avail_override_flag        : 1;
                unsigned int    pred_avail_flags                : 8;
            } bits;
            unsigned int value;
        } intra_fields;
        

        
        
        struct {
            union {
            } bits;
            unsigned int value;
        } inter_fields;
        
    } info;
} VAEncMacroblockParameterBufferH264;


#ifdef __cplusplus
}
#endif

#endif 
