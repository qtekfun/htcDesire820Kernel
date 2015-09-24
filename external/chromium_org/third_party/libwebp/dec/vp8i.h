// Copyright 2010 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#ifndef WEBP_DEC_VP8I_H_
#define WEBP_DEC_VP8I_H_

#include <string.h>     
#include "./vp8li.h"
#include "../utils/bit_reader.h"
#include "../utils/thread.h"
#include "../dsp/dsp.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif


#define DEC_MAJ_VERSION 0
#define DEC_MIN_VERSION 3
#define DEC_REV_VERSION 1

#define ONLY_KEYFRAME_CODE      

enum { B_DC_PRED = 0,   
       B_TM_PRED,
       B_VE_PRED,
       B_HE_PRED,
       B_RD_PRED,
       B_VR_PRED,
       B_LD_PRED,
       B_VL_PRED,
       B_HD_PRED,
       B_HU_PRED,
       NUM_BMODES = B_HU_PRED + 1 - B_DC_PRED,  

       
       DC_PRED = B_DC_PRED, V_PRED = B_VE_PRED,
       H_PRED = B_HE_PRED, TM_PRED = B_TM_PRED,
       B_PRED = NUM_BMODES,   

       
       B_DC_PRED_NOTOP = 4,
       B_DC_PRED_NOLEFT = 5,
       B_DC_PRED_NOTOPLEFT = 6,
       NUM_B_DC_MODES = 7 };

enum { MB_FEATURE_TREE_PROBS = 3,
       NUM_MB_SEGMENTS = 4,
       NUM_REF_LF_DELTAS = 4,
       NUM_MODE_LF_DELTAS = 4,    
       MAX_NUM_PARTITIONS = 8,
       
       NUM_TYPES = 4,
       NUM_BANDS = 8,
       NUM_CTX = 3,
       NUM_PROBAS = 11,
       NUM_MV_PROBAS = 19 };

#define BPS       32    
#define YUV_SIZE (BPS * 17 + BPS * 9)
#define Y_SIZE   (BPS * 17)
#define Y_OFF    (BPS * 1 + 8)
#define U_OFF    (Y_OFF + BPS * 16 + BPS)
#define V_OFF    (U_OFF + 16)


typedef struct {
  uint8_t key_frame_;
  uint8_t profile_;
  uint8_t show_;
  uint32_t partition_length_;
} VP8FrameHeader;

typedef struct {
  uint16_t width_;
  uint16_t height_;
  uint8_t xscale_;
  uint8_t yscale_;
  uint8_t colorspace_;   
  uint8_t clamp_type_;
} VP8PictureHeader;

typedef struct {
  int use_segment_;
  int update_map_;        
  int absolute_delta_;    
  int8_t quantizer_[NUM_MB_SEGMENTS];        
  int8_t filter_strength_[NUM_MB_SEGMENTS];  
} VP8SegmentHeader;

typedef struct {
  uint8_t segments_[MB_FEATURE_TREE_PROBS];
  
  uint8_t coeffs_[NUM_TYPES][NUM_BANDS][NUM_CTX][NUM_PROBAS];
#ifndef ONLY_KEYFRAME_CODE
  uint8_t ymode_[4], uvmode_[3];
  uint8_t mv_[2][NUM_MV_PROBAS];
#endif
} VP8Proba;

typedef struct {
  int simple_;                  
  int level_;                   
  int sharpness_;               
  int use_lf_delta_;
  int ref_lf_delta_[NUM_REF_LF_DELTAS];
  int mode_lf_delta_[NUM_MODE_LF_DELTAS];
} VP8FilterHeader;


typedef struct {  
  unsigned int f_level_:6;      
  unsigned int f_ilevel_:6;     
  unsigned int f_inner_:1;      
} VP8FInfo;

typedef struct {  
  unsigned int nz_:24;       
  unsigned int dc_nz_:1;     
  unsigned int skip_:1;      
} VP8MB;

typedef int quant_t[2];      
typedef struct {
  quant_t y1_mat_, y2_mat_, uv_mat_;
} VP8QuantMatrix;

typedef struct {
  int id_;            
  int mb_y_;          
  int filter_row_;    
  VP8FInfo* f_info_;  
  VP8Io io_;          
} VP8ThreadContext;


struct VP8Decoder {
  VP8StatusCode status_;
  int ready_;     
  const char* error_msg_;  

  
  VP8BitReader br_;

  
  VP8FrameHeader   frm_hdr_;
  VP8PictureHeader pic_hdr_;
  VP8FilterHeader  filter_hdr_;
  VP8SegmentHeader segment_hdr_;

  
  WebPWorker worker_;
  int use_threads_;    
  int cache_id_;       
  int num_caches_;     
  VP8ThreadContext thread_ctx_;  

  
  int mb_w_, mb_h_;

  
  int tl_mb_x_, tl_mb_y_;  
  int br_mb_x_, br_mb_y_;  

  
  int num_parts_;
  
  VP8BitReader parts_[MAX_NUM_PARTITIONS];

  
  
  
  
  
  uint32_t buffer_flags_;

  
  VP8QuantMatrix dqm_[NUM_MB_SEGMENTS];

  
  VP8Proba proba_;
  int use_skip_proba_;
  uint8_t skip_p_;
#ifndef ONLY_KEYFRAME_CODE
  uint8_t intra_p_, last_p_, golden_p_;
  VP8Proba proba_saved_;
  int update_proba_;
#endif

  
  uint8_t* intra_t_;     
  uint8_t  intra_l_[4];  
  uint8_t* y_t_;         
  uint8_t* u_t_, *v_t_;  

  VP8MB* mb_info_;       
  VP8FInfo* f_info_;     
  uint8_t* yuv_b_;       
  int16_t* coeffs_;      

  uint8_t* cache_y_;     
  uint8_t* cache_u_;
  uint8_t* cache_v_;
  int cache_y_stride_;
  int cache_uv_stride_;

  
  void* mem_;
  size_t mem_size_;

  
  int mb_x_, mb_y_;       
  uint8_t is_i4x4_;       
  uint8_t imodes_[16];    
  uint8_t uvmode_;        
  uint8_t segment_;       

  
  
  
  
  uint32_t non_zero_;
  uint32_t non_zero_ac_;

  
  int filter_type_;                          
  int filter_row_;                           
  VP8FInfo fstrengths_[NUM_MB_SEGMENTS][2];  

  
  const uint8_t* alpha_data_;   
  size_t alpha_data_size_;
  int is_alpha_decoded_;  
  uint8_t* alpha_plane_;        

  int layer_colorspace_;
  const uint8_t* layer_data_;   
  size_t layer_data_size_;
};


int VP8SetError(VP8Decoder* const dec,
                VP8StatusCode error, const char* const msg);

void VP8ResetProba(VP8Proba* const proba);
void VP8ParseProba(VP8BitReader* const br, VP8Decoder* const dec);
void VP8ParseIntraMode(VP8BitReader* const br,  VP8Decoder* const dec);

void VP8ParseQuant(VP8Decoder* const dec);

int VP8InitFrame(VP8Decoder* const dec, VP8Io* io);
void VP8ReconstructBlock(VP8Decoder* const dec);
VP8StatusCode VP8EnterCritical(VP8Decoder* const dec, VP8Io* const io);
int VP8ExitCritical(VP8Decoder* const dec, VP8Io* const io);
int VP8ProcessRow(VP8Decoder* const dec, VP8Io* const io);
void VP8InitScanline(VP8Decoder* const dec);
int VP8DecodeMB(VP8Decoder* const dec, VP8BitReader* const token_br);

const uint8_t* VP8DecompressAlphaRows(VP8Decoder* const dec,
                                      int row, int num_rows);

int VP8DecodeLayer(VP8Decoder* const dec);


#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif

#endif  
