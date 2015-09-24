// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#ifndef WEBP_ENC_VP8ENCI_H_
#define WEBP_ENC_VP8ENCI_H_

#include <string.h>     
#include "../webp/encode.h"
#include "../dsp/dsp.h"
#include "../utils/bit_writer.h"
#include "../utils/thread.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif


#define ENC_MAJ_VERSION 0
#define ENC_MIN_VERSION 3
#define ENC_REV_VERSION 1

enum { B_DC_PRED = 0,   
       B_TM_PRED = 1,
       B_VE_PRED = 2,
       B_HE_PRED = 3,
       B_RD_PRED = 4,
       B_VR_PRED = 5,
       B_LD_PRED = 6,
       B_VL_PRED = 7,
       B_HD_PRED = 8,
       B_HU_PRED = 9,
       NUM_BMODES = B_HU_PRED + 1 - B_DC_PRED,  

       
       DC_PRED = B_DC_PRED, V_PRED = B_VE_PRED,
       H_PRED = B_HE_PRED, TM_PRED = B_TM_PRED,
       NUM_PRED_MODES = 4
     };

enum { NUM_MB_SEGMENTS = 4,
       MAX_NUM_PARTITIONS = 8,
       NUM_TYPES = 4,   
       NUM_BANDS = 8,
       NUM_CTX = 3,
       NUM_PROBAS = 11,
       MAX_LF_LEVELS = 64,       
       MAX_VARIABLE_LEVEL = 67,  
       MAX_LEVEL = 2047          
     };

typedef enum {   
  RD_OPT_NONE        = 0,  
  RD_OPT_BASIC       = 1,  
  RD_OPT_TRELLIS     = 2,  
  RD_OPT_TRELLIS_ALL = 3   
} VP8RDLevel;

#define BPS       16   
#define Y_SIZE   (BPS * 16)
#define UV_SIZE  (BPS * 8)
#define YUV_SIZE (Y_SIZE + UV_SIZE)
#define PRED_SIZE (6 * 16 * BPS + 12 * BPS)
#define Y_OFF    (0)
#define U_OFF    (Y_SIZE)
#define V_OFF    (U_OFF + 8)
#define ALIGN_CST 15
#define DO_ALIGN(PTR) ((uintptr_t)((PTR) + ALIGN_CST) & ~ALIGN_CST)

extern const int VP8Scan[16 + 4 + 4];           
extern const int VP8UVModeOffsets[4];           
extern const int VP8I16ModeOffsets[4];
extern const int VP8I4ModeOffsets[NUM_BMODES];

#define I16DC16 (0 * 16 * BPS)
#define I16TM16 (1 * 16 * BPS)
#define I16VE16 (2 * 16 * BPS)
#define I16HE16 (3 * 16 * BPS)
#define C8DC8 (4 * 16 * BPS)
#define C8TM8 (4 * 16 * BPS + 8 * BPS)
#define C8VE8 (5 * 16 * BPS)
#define C8HE8 (5 * 16 * BPS + 8 * BPS)
#define I4DC4 (6 * 16 * BPS +  0)
#define I4TM4 (6 * 16 * BPS +  4)
#define I4VE4 (6 * 16 * BPS +  8)
#define I4HE4 (6 * 16 * BPS + 12)
#define I4RD4 (6 * 16 * BPS + 4 * BPS +  0)
#define I4VR4 (6 * 16 * BPS + 4 * BPS +  4)
#define I4LD4 (6 * 16 * BPS + 4 * BPS +  8)
#define I4VL4 (6 * 16 * BPS + 4 * BPS + 12)
#define I4HD4 (6 * 16 * BPS + 8 * BPS +  0)
#define I4HU4 (6 * 16 * BPS + 8 * BPS +  4)
#define I4TMP (6 * 16 * BPS + 8 * BPS +  8)

typedef int64_t score_t;     
#define MAX_COST ((score_t)0x7fffffffffffffLL)

#define QFIX 17
#define BIAS(b)  ((b) << (QFIX - 8))
static WEBP_INLINE int QUANTDIV(int n, int iQ, int B) {
  return (n * iQ + B) >> QFIX;
}

#define MAX_COEFF_THRESH   31
typedef struct VP8Histogram VP8Histogram;
struct VP8Histogram {
  
  int distribution[MAX_COEFF_THRESH + 1];
};



typedef uint32_t proba_t;   
typedef uint8_t ProbaArray[NUM_CTX][NUM_PROBAS];
typedef proba_t StatsArray[NUM_CTX][NUM_PROBAS];
typedef uint16_t CostArray[NUM_CTX][MAX_VARIABLE_LEVEL + 1];
typedef double LFStats[NUM_MB_SEGMENTS][MAX_LF_LEVELS];  

typedef struct VP8Encoder VP8Encoder;

typedef struct {
  int num_segments_;      
  int update_map_;        
                          
  int size_;              
} VP8SegmentHeader;

typedef struct {
  uint8_t segments_[3];     
  uint8_t skip_proba_;      
  ProbaArray coeffs_[NUM_TYPES][NUM_BANDS];      
  StatsArray stats_[NUM_TYPES][NUM_BANDS];       
  CostArray level_cost_[NUM_TYPES][NUM_BANDS];   
  int dirty_;               
  int use_skip_proba_;      
  int nb_skip_;             
} VP8Proba;

typedef struct {
  int simple_;             
  int level_;              
  int sharpness_;          
  int i4x4_lf_delta_;      
} VP8FilterHeader;


typedef struct {
  
  unsigned int type_:2;     
  unsigned int uv_mode_:2;
  unsigned int skip_:1;
  unsigned int segment_:2;
  uint8_t alpha_;      
} VP8MBInfo;

typedef struct VP8Matrix {
  uint16_t q_[16];        
  uint16_t iq_[16];       
  uint16_t bias_[16];     
  uint16_t zthresh_[16];  
  uint16_t sharpen_[16];  
} VP8Matrix;

typedef struct {
  VP8Matrix y1_, y2_, uv_;  
  int alpha_;      
                   
  int beta_;       
  int quant_;      
  int fstrength_;  
  
  int lambda_i16_, lambda_i4_, lambda_uv_;
  int lambda_mode_, lambda_trellis_, tlambda_;
  int lambda_trellis_i16_, lambda_trellis_i4_, lambda_trellis_uv_;
} VP8SegmentInfo;

typedef struct {
  score_t D, SD, R, score;    
  int16_t y_dc_levels[16];    
  int16_t y_ac_levels[16][16];
  int16_t uv_levels[4 + 4][16];
  int mode_i16;               
  uint8_t modes_i4[16];       
  int mode_uv;                
  uint32_t nz;                
} VP8ModeScore;

typedef struct {
  int x_, y_;                      
  int y_offset_, uv_offset_;       
  int y_stride_, uv_stride_;       
  uint8_t*      yuv_in_;           
  uint8_t*      yuv_out_;          
  uint8_t*      yuv_out2_;         
  uint8_t*      yuv_p_;            
  VP8Encoder*   enc_;              
  VP8MBInfo*    mb_;               
  VP8BitWriter* bw_;               
  uint8_t*      preds_;            
  uint32_t*     nz_;               
  uint8_t       i4_boundary_[37];  
  uint8_t*      i4_top_;           
  int           i4_;               
  int           top_nz_[9];        
  int           left_nz_[9];       
  uint64_t      bit_count_[4][3];  
  uint64_t      luma_bits_;        
  uint64_t      uv_bits_;          
  LFStats*      lf_stats_;         
  int           do_trellis_;       
  int           done_;             
  int           percent0_;         
} VP8EncIterator;

  
void VP8IteratorInit(VP8Encoder* const enc, VP8EncIterator* const it);
void VP8IteratorReset(VP8EncIterator* const it);
void VP8IteratorImport(const VP8EncIterator* const it);
void VP8IteratorExport(const VP8EncIterator* const it);
int VP8IteratorNext(VP8EncIterator* const it,
                    const uint8_t* const block_to_save);
int VP8IteratorProgress(const VP8EncIterator* const it,
                        int final_delta_percent);
void VP8IteratorStartI4(VP8EncIterator* const it);
int VP8IteratorRotateI4(VP8EncIterator* const it,
                        const uint8_t* const yuv_out);

void VP8IteratorNzToBytes(VP8EncIterator* const it);
void VP8IteratorBytesToNz(VP8EncIterator* const it);

void VP8SetIntra16Mode(const VP8EncIterator* const it, int mode);
void VP8SetIntra4Mode(const VP8EncIterator* const it, const uint8_t* modes);
void VP8SetIntraUVMode(const VP8EncIterator* const it, int mode);
void VP8SetSkip(const VP8EncIterator* const it, int skip);
void VP8SetSegment(const VP8EncIterator* const it, int segment);


typedef struct VP8Tokens VP8Tokens;  

typedef struct {
#if !defined(DISABLE_TOKEN_BUFFER)
  VP8Tokens* pages_;        
  VP8Tokens** last_page_;   
  uint16_t* tokens_;        
  int left_;          
#endif
  int error_;         
} VP8TBuffer;

void VP8TBufferInit(VP8TBuffer* const b);    
void VP8TBufferClear(VP8TBuffer* const b);   

#if !defined(DISABLE_TOKEN_BUFFER)

int VP8EmitTokens(VP8TBuffer* const b, VP8BitWriter* const bw,
                  const uint8_t* const probas, int final_pass);

int VP8RecordCoeffTokens(int ctx, int coeff_type, int first, int last,
                         const int16_t* const coeffs,
                         VP8TBuffer* const tokens);

void VP8TokenToStats(const VP8TBuffer* const b, proba_t* const stats);

#endif  


struct VP8Encoder {
  const WebPConfig* config_;    
  WebPPicture* pic_;            

  
  VP8FilterHeader   filter_hdr_;     
  VP8SegmentHeader  segment_hdr_;    

  int profile_;                      

  
  int mb_w_, mb_h_;
  int preds_w_;   

  
  int num_parts_;

  
  VP8BitWriter bw_;                         
  VP8BitWriter parts_[MAX_NUM_PARTITIONS];  
  VP8TBuffer tokens_;                       

  int percent_;                             

  
  int has_alpha_;
  uint8_t* alpha_data_;       
  uint32_t alpha_data_size_;
  WebPWorker alpha_worker_;

  
  int use_layer_;
  VP8BitWriter layer_bw_;
  uint8_t* layer_data_;
  size_t layer_data_size_;

  
  VP8SegmentInfo dqm_[NUM_MB_SEGMENTS];
  int base_quant_;                 
                                   
  int alpha_;                      
  int uv_alpha_;                   
  
  int dq_y1_dc_;
  int dq_y2_dc_, dq_y2_ac_;
  int dq_uv_dc_, dq_uv_ac_;

  
  VP8Proba proba_;
  uint64_t sse_[4];        
  uint64_t sse_count_;     
  int      coded_size_;
  int      residual_bytes_[3][4];
  int      block_count_[3];

  
  int method_;               
  VP8RDLevel rd_opt_level_;  
  int max_i4_header_bits_;   
  int thread_level_;         
  int do_search_;            
  int use_tokens_;           

  
  VP8MBInfo* mb_info_;   
  uint8_t*   preds_;     
  uint32_t*  nz_;        
  uint8_t*   yuv_in_;    
  uint8_t*   yuv_out_;   
  uint8_t*   yuv_out2_;  
  uint8_t*   yuv_p_;     
  uint8_t   *y_top_;     
  uint8_t   *uv_top_;    
                         
  uint8_t   *y_left_;    
  uint8_t   *u_left_;    
  uint8_t   *v_left_;    

  LFStats   *lf_stats_;  
};


  
extern const uint8_t VP8CoeffsProba0[NUM_TYPES][NUM_BANDS][NUM_CTX][NUM_PROBAS];
extern const uint8_t
    VP8CoeffsUpdateProba[NUM_TYPES][NUM_BANDS][NUM_CTX][NUM_PROBAS];
void VP8DefaultProbas(VP8Encoder* const enc);
void VP8WriteProbas(VP8BitWriter* const bw, const VP8Proba* const probas);
void VP8CodeIntraModes(VP8Encoder* const enc);

  
int VP8EncWrite(VP8Encoder* const enc);
void VP8EncFreeBitWriters(VP8Encoder* const enc);

  
extern const uint8_t VP8EncBands[16 + 1];
extern const uint8_t VP8Cat3[];
extern const uint8_t VP8Cat4[];
extern const uint8_t VP8Cat5[];
extern const uint8_t VP8Cat6[];

void VP8MakeLuma16Preds(const VP8EncIterator* const it);
void VP8MakeChroma8Preds(const VP8EncIterator* const it);
void VP8MakeIntra4Preds(const VP8EncIterator* const it);
int VP8GetCostLuma16(VP8EncIterator* const it, const VP8ModeScore* const rd);
int VP8GetCostLuma4(VP8EncIterator* const it, const int16_t levels[16]);
int VP8GetCostUV(VP8EncIterator* const it, const VP8ModeScore* const rd);
int VP8EncLoop(VP8Encoder* const enc);
int VP8EncTokenLoop(VP8Encoder* const enc);

  
int WebPEncodingSetError(const WebPPicture* const pic, WebPEncodingError error);
int WebPReportProgress(const WebPPicture* const pic,
                       int percent, int* const percent_store);

  
int VP8EncAnalyze(VP8Encoder* const enc);

  
void VP8SetSegmentParams(VP8Encoder* const enc, float quality);
int VP8Decimate(VP8EncIterator* const it, VP8ModeScore* const rd,
                VP8RDLevel rd_opt);

  
void VP8EncInitAlpha(VP8Encoder* const enc);    
int VP8EncStartAlpha(VP8Encoder* const enc);    
int VP8EncFinishAlpha(VP8Encoder* const enc);   
int VP8EncDeleteAlpha(VP8Encoder* const enc);   

  
void VP8EncInitLayer(VP8Encoder* const enc);     
void VP8EncCodeLayerBlock(VP8EncIterator* it);   
int VP8EncFinishLayer(VP8Encoder* const enc);    
void VP8EncDeleteLayer(VP8Encoder* enc);         

  

typedef struct {
  double w, xm, ym, xxm, xym, yym;
} DistoStats;
void VP8SSIMAddStats(const DistoStats* const src, DistoStats* const dst);
void VP8SSIMAccumulatePlane(const uint8_t* src1, int stride1,
                            const uint8_t* src2, int stride2,
                            int W, int H, DistoStats* const stats);
double VP8SSIMGet(const DistoStats* const stats);
double VP8SSIMGetSquaredError(const DistoStats* const stats);

void VP8InitFilter(VP8EncIterator* const it);
void VP8StoreFilterStats(VP8EncIterator* const it);
void VP8AdjustFilterStrength(VP8EncIterator* const it);


#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif

#endif  
