// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#include <assert.h>
#include <math.h>

#include "./vp8enci.h"
#include "./cost.h"

#define DO_TRELLIS_I4  1
#define DO_TRELLIS_I16 1   
#define DO_TRELLIS_UV  0   
#define USE_TDISTO 1

#define MID_ALPHA 64      
#define MIN_ALPHA 30      
#define MAX_ALPHA 100     

#define SNS_TO_DQ 0.9     
                          

#define I4_PENALTY 4000   

#define MULT_8B(a, b) (((a) * (b) + 128) >> 8)

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif


static WEBP_INLINE int clip(int v, int m, int M) {
  return v < m ? m : v > M ? M : v;
}

static const uint8_t kZigzag[16] = {
  0, 1, 4, 8, 5, 2, 3, 6, 9, 12, 13, 10, 7, 11, 14, 15
};

static const uint8_t kDcTable[128] = {
  4,     5,   6,   7,   8,   9,  10,  10,
  11,   12,  13,  14,  15,  16,  17,  17,
  18,   19,  20,  20,  21,  21,  22,  22,
  23,   23,  24,  25,  25,  26,  27,  28,
  29,   30,  31,  32,  33,  34,  35,  36,
  37,   37,  38,  39,  40,  41,  42,  43,
  44,   45,  46,  46,  47,  48,  49,  50,
  51,   52,  53,  54,  55,  56,  57,  58,
  59,   60,  61,  62,  63,  64,  65,  66,
  67,   68,  69,  70,  71,  72,  73,  74,
  75,   76,  76,  77,  78,  79,  80,  81,
  82,   83,  84,  85,  86,  87,  88,  89,
  91,   93,  95,  96,  98, 100, 101, 102,
  104, 106, 108, 110, 112, 114, 116, 118,
  122, 124, 126, 128, 130, 132, 134, 136,
  138, 140, 143, 145, 148, 151, 154, 157
};

static const uint16_t kAcTable[128] = {
  4,     5,   6,   7,   8,   9,  10,  11,
  12,   13,  14,  15,  16,  17,  18,  19,
  20,   21,  22,  23,  24,  25,  26,  27,
  28,   29,  30,  31,  32,  33,  34,  35,
  36,   37,  38,  39,  40,  41,  42,  43,
  44,   45,  46,  47,  48,  49,  50,  51,
  52,   53,  54,  55,  56,  57,  58,  60,
  62,   64,  66,  68,  70,  72,  74,  76,
  78,   80,  82,  84,  86,  88,  90,  92,
  94,   96,  98, 100, 102, 104, 106, 108,
  110, 112, 114, 116, 119, 122, 125, 128,
  131, 134, 137, 140, 143, 146, 149, 152,
  155, 158, 161, 164, 167, 170, 173, 177,
  181, 185, 189, 193, 197, 201, 205, 209,
  213, 217, 221, 225, 229, 234, 239, 245,
  249, 254, 259, 264, 269, 274, 279, 284
};

static const uint16_t kAcTable2[128] = {
  8,     8,   9,  10,  12,  13,  15,  17,
  18,   20,  21,  23,  24,  26,  27,  29,
  31,   32,  34,  35,  37,  38,  40,  41,
  43,   44,  46,  48,  49,  51,  52,  54,
  55,   57,  58,  60,  62,  63,  65,  66,
  68,   69,  71,  72,  74,  75,  77,  79,
  80,   82,  83,  85,  86,  88,  89,  93,
  96,   99, 102, 105, 108, 111, 114, 117,
  120, 124, 127, 130, 133, 136, 139, 142,
  145, 148, 151, 155, 158, 161, 164, 167,
  170, 173, 176, 179, 184, 189, 193, 198,
  203, 207, 212, 217, 221, 226, 230, 235,
  240, 244, 249, 254, 258, 263, 268, 274,
  280, 286, 292, 299, 305, 311, 317, 323,
  330, 336, 342, 348, 354, 362, 370, 379,
  385, 393, 401, 409, 416, 424, 432, 440
};

static const uint16_t kCoeffThresh[16] = {
  0,  10, 20, 30,
  10, 20, 30, 30,
  20, 30, 30, 30,
  30, 30, 30, 30
};

static const uint8_t kBiasMatrices[3][16] = {  
  { 96, 96, 96, 96,
    96, 96, 96, 96,
    96, 96, 96, 96,
    96, 96, 96, 96 },
  { 96, 96, 96, 96,
    96, 96, 96, 96,
    96, 96, 96, 96,
    96, 96, 96, 96 },
  { 96, 96, 96, 96,
    96, 96, 96, 96,
    96, 96, 96, 96,
    96, 96, 96, 96 }
};

static const uint8_t kFreqSharpening[16] = {
  0,  30, 60, 90,
  30, 60, 90, 90,
  60, 90, 90, 90,
  90, 90, 90, 90
};


static int ExpandMatrix(VP8Matrix* const m, int type) {
  int i;
  int sum = 0;
  for (i = 2; i < 16; ++i) {
    m->q_[i] = m->q_[1];
  }
  for (i = 0; i < 16; ++i) {
    const int j = kZigzag[i];
    const int bias = kBiasMatrices[type][j];
    m->iq_[j] = (1 << QFIX) / m->q_[j];
    m->bias_[j] = BIAS(bias);
    
    m->zthresh_[j] = ((256  - bias) * m->q_[j] + 127) >> 8;
    m->sharpen_[j] = (kFreqSharpening[j] * m->q_[j]) >> 11;
    sum += m->q_[j];
  }
  return (sum + 8) >> 4;
}

static void SetupMatrices(VP8Encoder* enc) {
  int i;
  const int tlambda_scale =
    (enc->method_ >= 4) ? enc->config_->sns_strength
                        : 0;
  const int num_segments = enc->segment_hdr_.num_segments_;
  for (i = 0; i < num_segments; ++i) {
    VP8SegmentInfo* const m = &enc->dqm_[i];
    const int q = m->quant_;
    int q4, q16, quv;
    m->y1_.q_[0] = kDcTable[clip(q + enc->dq_y1_dc_, 0, 127)];
    m->y1_.q_[1] = kAcTable[clip(q,                  0, 127)];

    m->y2_.q_[0] = kDcTable[ clip(q + enc->dq_y2_dc_, 0, 127)] * 2;
    m->y2_.q_[1] = kAcTable2[clip(q + enc->dq_y2_ac_, 0, 127)];

    m->uv_.q_[0] = kDcTable[clip(q + enc->dq_uv_dc_, 0, 117)];
    m->uv_.q_[1] = kAcTable[clip(q + enc->dq_uv_ac_, 0, 127)];

    q4  = ExpandMatrix(&m->y1_, 0);
    q16 = ExpandMatrix(&m->y2_, 1);
    quv = ExpandMatrix(&m->uv_, 2);

    
    {
      m->lambda_i4_  = (3 * q4 * q4) >> 7;
      m->lambda_i16_ = (3 * q16 * q16);
      m->lambda_uv_  = (3 * quv * quv) >> 6;
      m->lambda_mode_    = (1 * q4 * q4) >> 7;
      m->lambda_trellis_i4_  = (7 * q4 * q4) >> 3;
      m->lambda_trellis_i16_ = (q16 * q16) >> 2;
      m->lambda_trellis_uv_  = (quv *quv) << 1;
      m->tlambda_            = (tlambda_scale * q4) >> 5;
    }
  }
}


#define FSTRENGTH_CUTOFF 3

static void SetupFilterStrength(VP8Encoder* const enc) {
  int i;
  const int level0 = enc->config_->filter_strength;
  for (i = 0; i < NUM_MB_SEGMENTS; ++i) {
    
    const int level = level0 * 256 * enc->dqm_[i].quant_ / 128;
    const int f = level / (256 + enc->dqm_[i].beta_);
    enc->dqm_[i].fstrength_ = (f < FSTRENGTH_CUTOFF) ? 0 : (f > 63) ? 63 : f;
  }
  
  enc->filter_hdr_.level_ = enc->dqm_[0].fstrength_;
  enc->filter_hdr_.simple_ = (enc->config_->filter_type == 0);
  enc->filter_hdr_.sharpness_ = enc->config_->filter_sharpness;
}


#define MAX_DQ_UV (6)
#define MIN_DQ_UV (-4)

static double QualityToCompression(double c) {
  const double linear_c = (c < 0.75) ? c * (2. / 3.) : 2. * c - 1.;
  
  
  
  
  
  
  const double v = pow(linear_c, 1 / 3.);
  return v;
}

static double QualityToJPEGCompression(double c, double alpha) {
  
  
  
  
  const double amin = 0.30;
  const double amax = 0.85;
  const double exp_min = 0.4;
  const double exp_max = 0.9;
  const double slope = (exp_min - exp_max) / (amax - amin);
  
  
  const double expn = (alpha > amax) ? exp_min
                    : (alpha < amin) ? exp_max
                    : exp_max + slope * (alpha - amin);
  const double v = pow(c, expn);
  return v;
}

static int SegmentsAreEquivalent(const VP8SegmentInfo* const S1,
                                 const VP8SegmentInfo* const S2) {
  return (S1->quant_ == S2->quant_) && (S1->fstrength_ == S2->fstrength_);
}

static void SimplifySegments(VP8Encoder* const enc) {
  int map[NUM_MB_SEGMENTS] = { 0, 1, 2, 3 };
  const int num_segments = enc->segment_hdr_.num_segments_;
  int num_final_segments = 1;
  int s1, s2;
  for (s1 = 1; s1 < num_segments; ++s1) {    
    const VP8SegmentInfo* const S1 = &enc->dqm_[s1];
    int found = 0;
    
    for (s2 = 0; s2 < num_final_segments; ++s2) {
      const VP8SegmentInfo* const S2 = &enc->dqm_[s2];
      if (SegmentsAreEquivalent(S1, S2)) {
        found = 1;
        break;
      }
    }
    map[s1] = s2;
    if (!found) {
      if (num_final_segments != s1) {
        enc->dqm_[num_final_segments] = enc->dqm_[s1];
      }
      ++num_final_segments;
    }
  }
  if (num_final_segments < num_segments) {  
    int i = enc->mb_w_ * enc->mb_h_;
    while (i-- > 0) enc->mb_info_[i].segment_ = map[enc->mb_info_[i].segment_];
    enc->segment_hdr_.num_segments_ = num_final_segments;
    
    for (i = num_final_segments; i < num_segments; ++i) {
      enc->dqm_[i] = enc->dqm_[num_final_segments - 1];
    }
  }
}

void VP8SetSegmentParams(VP8Encoder* const enc, float quality) {
  int i;
  int dq_uv_ac, dq_uv_dc;
  const int num_segments = enc->segment_hdr_.num_segments_;
  const double amp = SNS_TO_DQ * enc->config_->sns_strength / 100. / 128.;
  const double Q = quality / 100.;
  const double c_base = enc->config_->emulate_jpeg_size ?
      QualityToJPEGCompression(Q, enc->alpha_ / 255.) :
      QualityToCompression(Q);
  for (i = 0; i < num_segments; ++i) {
    
    
    const double expn = 1. - amp * enc->dqm_[i].alpha_;
    const double c = pow(c_base, expn);
    const int q = (int)(127. * (1. - c));
    assert(expn > 0.);
    enc->dqm_[i].quant_ = clip(q, 0, 127);
  }

  
  enc->base_quant_ = enc->dqm_[0].quant_;

  
  for (i = num_segments; i < NUM_MB_SEGMENTS; ++i) {
    enc->dqm_[i].quant_ = enc->base_quant_;
  }

  
  
  
  dq_uv_ac = (enc->uv_alpha_ - MID_ALPHA) * (MAX_DQ_UV - MIN_DQ_UV)
                                          / (MAX_ALPHA - MIN_ALPHA);
  
  dq_uv_ac = dq_uv_ac * enc->config_->sns_strength / 100;
  
  dq_uv_ac = clip(dq_uv_ac, MIN_DQ_UV, MAX_DQ_UV);
  
  
  
  dq_uv_dc = -4 * enc->config_->sns_strength / 100;
  dq_uv_dc = clip(dq_uv_dc, -15, 15);   

  enc->dq_y1_dc_ = 0;       
  enc->dq_y2_dc_ = 0;
  enc->dq_y2_ac_ = 0;
  enc->dq_uv_dc_ = dq_uv_dc;
  enc->dq_uv_ac_ = dq_uv_ac;

  SetupFilterStrength(enc);   

  if (num_segments > 1) SimplifySegments(enc);

  SetupMatrices(enc);         
}


const int VP8I16ModeOffsets[4] = { I16DC16, I16TM16, I16VE16, I16HE16 };
const int VP8UVModeOffsets[4] = { C8DC8, C8TM8, C8VE8, C8HE8 };

const int VP8I4ModeOffsets[NUM_BMODES] = {
  I4DC4, I4TM4, I4VE4, I4HE4, I4RD4, I4VR4, I4LD4, I4VL4, I4HD4, I4HU4
};

void VP8MakeLuma16Preds(const VP8EncIterator* const it) {
  const VP8Encoder* const enc = it->enc_;
  const uint8_t* const left = it->x_ ? enc->y_left_ : NULL;
  const uint8_t* const top = it->y_ ? enc->y_top_ + it->x_ * 16 : NULL;
  VP8EncPredLuma16(it->yuv_p_, left, top);
}

void VP8MakeChroma8Preds(const VP8EncIterator* const it) {
  const VP8Encoder* const enc = it->enc_;
  const uint8_t* const left = it->x_ ? enc->u_left_ : NULL;
  const uint8_t* const top = it->y_ ? enc->uv_top_ + it->x_ * 16 : NULL;
  VP8EncPredChroma8(it->yuv_p_, left, top);
}

void VP8MakeIntra4Preds(const VP8EncIterator* const it) {
  VP8EncPredLuma4(it->yuv_p_, it->i4_top_);
}



const int VP8Scan[16 + 4 + 4] = {
  
  0 +  0 * BPS,  4 +  0 * BPS, 8 +  0 * BPS, 12 +  0 * BPS,
  0 +  4 * BPS,  4 +  4 * BPS, 8 +  4 * BPS, 12 +  4 * BPS,
  0 +  8 * BPS,  4 +  8 * BPS, 8 +  8 * BPS, 12 +  8 * BPS,
  0 + 12 * BPS,  4 + 12 * BPS, 8 + 12 * BPS, 12 + 12 * BPS,

  0 + 0 * BPS,   4 + 0 * BPS, 0 + 4 * BPS,  4 + 4 * BPS,    
  8 + 0 * BPS,  12 + 0 * BPS, 8 + 4 * BPS, 12 + 4 * BPS     
};


static const uint16_t kWeightY[16] = {
  38, 32, 20, 9, 32, 28, 17, 7, 20, 17, 10, 4, 9, 7, 4, 2
};

static const uint16_t kWeightTrellis[16] = {
#if USE_TDISTO == 0
  16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16
#else
  30, 27, 19, 11,
  27, 24, 17, 10,
  19, 17, 12,  8,
  11, 10,  8,  6
#endif
};

static void InitScore(VP8ModeScore* const rd) {
  rd->D  = 0;
  rd->SD = 0;
  rd->R  = 0;
  rd->nz = 0;
  rd->score = MAX_COST;
}

static void CopyScore(VP8ModeScore* const dst, const VP8ModeScore* const src) {
  dst->D  = src->D;
  dst->SD = src->SD;
  dst->R  = src->R;
  dst->nz = src->nz;      
  dst->score = src->score;
}

static void AddScore(VP8ModeScore* const dst, const VP8ModeScore* const src) {
  dst->D  += src->D;
  dst->SD += src->SD;
  dst->R  += src->R;
  dst->nz |= src->nz;     
  dst->score += src->score;
}



typedef struct {
  int prev;        
  int level;       
  int sign;        
  score_t cost;    
  score_t error;   
  int ctx;         
} Node;

#define MIN_DELTA 0   
#define MAX_DELTA 1   
#define NUM_NODES (MIN_DELTA + 1 + MAX_DELTA)
#define NODE(n, l) (nodes[(n) + 1][(l) + MIN_DELTA])

static WEBP_INLINE void SetRDScore(int lambda, VP8ModeScore* const rd) {
  
  rd->score = rd->R * lambda + 256 * (rd->D + rd->SD);
}

static WEBP_INLINE score_t RDScoreTrellis(int lambda, score_t rate,
                                          score_t distortion) {
  return rate * lambda + 256 * distortion;
}

static int TrellisQuantizeBlock(const VP8EncIterator* const it,
                                int16_t in[16], int16_t out[16],
                                int ctx0, int coeff_type,
                                const VP8Matrix* const mtx,
                                int lambda) {
  ProbaArray* const last_costs = it->enc_->proba_.coeffs_[coeff_type];
  CostArray* const costs = it->enc_->proba_.level_cost_[coeff_type];
  const int first = (coeff_type == 0) ? 1 : 0;
  Node nodes[17][NUM_NODES];
  int best_path[3] = {-1, -1, -1};   
  score_t best_score;
  int best_node;
  int last = first - 1;
  int n, m, p, nz;

  {
    score_t cost;
    score_t max_error;
    const int thresh = mtx->q_[1] * mtx->q_[1] / 4;
    const int last_proba = last_costs[VP8EncBands[first]][ctx0][0];

    
    max_error = 0;
    for (n = first; n < 16; ++n) {
      const int j  = kZigzag[n];
      const int err = in[j] * in[j];
      max_error += kWeightTrellis[j] * err;
      if (err > thresh) last = n;
    }
    
    
    if (last < 15) ++last;

    
    cost = VP8BitCost(0, last_proba);
    best_score = RDScoreTrellis(lambda, cost, max_error);

    
    n = first - 1;
    for (m = -MIN_DELTA; m <= MAX_DELTA; ++m) {
      NODE(n, m).cost = 0;
      NODE(n, m).error = max_error;
      NODE(n, m).ctx = ctx0;
    }
  }

  
  for (n = first; n <= last; ++n) {
    const int j  = kZigzag[n];
    const int Q  = mtx->q_[j];
    const int iQ = mtx->iq_[j];
    const int B = BIAS(0x00);     
    
    
    const int sign = (in[j] < 0);
    int coeff0 = (sign ? -in[j] : in[j]) + mtx->sharpen_[j];
    int level0;
    if (coeff0 > 2047) coeff0 = 2047;

    level0 = QUANTDIV(coeff0, iQ, B);
    
    for (m = -MIN_DELTA; m <= MAX_DELTA; ++m) {
      Node* const cur = &NODE(n, m);
      int delta_error, new_error;
      score_t cur_score = MAX_COST;
      int level = level0 + m;
      int last_proba;

      cur->sign = sign;
      cur->level = level;
      cur->ctx = (level == 0) ? 0 : (level == 1) ? 1 : 2;
      if (level >= 2048 || level < 0) {   
        cur->cost = MAX_COST;
        continue;
      }
      last_proba = last_costs[VP8EncBands[n + 1]][cur->ctx][0];

      
      
      new_error = coeff0 - level * Q;
      delta_error =
        kWeightTrellis[j] * (coeff0 * coeff0 - new_error * new_error);

      
      for (p = -MIN_DELTA; p <= MAX_DELTA; ++p) {
        const Node* const prev = &NODE(n - 1, p);
        const int prev_ctx = prev->ctx;
        const uint16_t* const tcost = costs[VP8EncBands[n]][prev_ctx];
        const score_t total_error = prev->error - delta_error;
        score_t cost, base_cost, score;

        if (prev->cost >= MAX_COST) {   
          continue;
        }

        
        base_cost = prev->cost + VP8LevelCost(tcost, level);

        
        cost = base_cost;
        if (level && n < 15) {
          cost += VP8BitCost(1, last_proba);
        }
        score = RDScoreTrellis(lambda, cost, total_error);
        if (score < cur_score) {
          cur_score = score;
          cur->cost  = cost;
          cur->error = total_error;
          cur->prev  = p;
        }

        
        if (level) {
          cost = base_cost;
          if (n < 15) cost += VP8BitCost(0, last_proba);
          score = RDScoreTrellis(lambda, cost, total_error);
          if (score < best_score) {
            best_score = score;
            best_path[0] = n;   
            best_path[1] = m;   
            best_path[2] = p;   
          }
        }
      }
    }
  }

  
  memset(in + first, 0, (16 - first) * sizeof(*in));
  memset(out + first, 0, (16 - first) * sizeof(*out));
  if (best_path[0] == -1) {
    return 0;   
  }

  
  
  
  n = best_path[0];
  best_node = best_path[1];
  NODE(n, best_node).prev = best_path[2];   
  nz = 0;

  for (; n >= first; --n) {
    const Node* const node = &NODE(n, best_node);
    const int j = kZigzag[n];
    out[n] = node->sign ? -node->level : node->level;
    nz |= (node->level != 0);
    in[j] = out[n] * mtx->q_[j];
    best_node = node->prev;
  }
  return nz;
}

#undef NODE


static int ReconstructIntra16(VP8EncIterator* const it,
                              VP8ModeScore* const rd,
                              uint8_t* const yuv_out,
                              int mode) {
  const VP8Encoder* const enc = it->enc_;
  const uint8_t* const ref = it->yuv_p_ + VP8I16ModeOffsets[mode];
  const uint8_t* const src = it->yuv_in_ + Y_OFF;
  const VP8SegmentInfo* const dqm = &enc->dqm_[it->mb_->segment_];
  int nz = 0;
  int n;
  int16_t tmp[16][16], dc_tmp[16];

  for (n = 0; n < 16; ++n) {
    VP8FTransform(src + VP8Scan[n], ref + VP8Scan[n], tmp[n]);
  }
  VP8FTransformWHT(tmp[0], dc_tmp);
  nz |= VP8EncQuantizeBlock(dc_tmp, rd->y_dc_levels, 0, &dqm->y2_) << 24;

  if (DO_TRELLIS_I16 && it->do_trellis_) {
    int x, y;
    VP8IteratorNzToBytes(it);
    for (y = 0, n = 0; y < 4; ++y) {
      for (x = 0; x < 4; ++x, ++n) {
        const int ctx = it->top_nz_[x] + it->left_nz_[y];
        const int non_zero =
           TrellisQuantizeBlock(it, tmp[n], rd->y_ac_levels[n], ctx, 0,
                                &dqm->y1_, dqm->lambda_trellis_i16_);
        it->top_nz_[x] = it->left_nz_[y] = non_zero;
        nz |= non_zero << n;
      }
    }
  } else {
    for (n = 0; n < 16; ++n) {
      nz |= VP8EncQuantizeBlock(tmp[n], rd->y_ac_levels[n], 1, &dqm->y1_) << n;
    }
  }

  
  VP8ITransformWHT(dc_tmp, tmp[0]);
  for (n = 0; n < 16; n += 2) {
    VP8ITransform(ref + VP8Scan[n], tmp[n], yuv_out + VP8Scan[n], 1);
  }

  return nz;
}

static int ReconstructIntra4(VP8EncIterator* const it,
                             int16_t levels[16],
                             const uint8_t* const src,
                             uint8_t* const yuv_out,
                             int mode) {
  const VP8Encoder* const enc = it->enc_;
  const uint8_t* const ref = it->yuv_p_ + VP8I4ModeOffsets[mode];
  const VP8SegmentInfo* const dqm = &enc->dqm_[it->mb_->segment_];
  int nz = 0;
  int16_t tmp[16];

  VP8FTransform(src, ref, tmp);
  if (DO_TRELLIS_I4 && it->do_trellis_) {
    const int x = it->i4_ & 3, y = it->i4_ >> 2;
    const int ctx = it->top_nz_[x] + it->left_nz_[y];
    nz = TrellisQuantizeBlock(it, tmp, levels, ctx, 3, &dqm->y1_,
                              dqm->lambda_trellis_i4_);
  } else {
    nz = VP8EncQuantizeBlock(tmp, levels, 0, &dqm->y1_);
  }
  VP8ITransform(ref, tmp, yuv_out, 0);
  return nz;
}

static int ReconstructUV(VP8EncIterator* const it, VP8ModeScore* const rd,
                         uint8_t* const yuv_out, int mode) {
  const VP8Encoder* const enc = it->enc_;
  const uint8_t* const ref = it->yuv_p_ + VP8UVModeOffsets[mode];
  const uint8_t* const src = it->yuv_in_ + U_OFF;
  const VP8SegmentInfo* const dqm = &enc->dqm_[it->mb_->segment_];
  int nz = 0;
  int n;
  int16_t tmp[8][16];

  for (n = 0; n < 8; ++n) {
    VP8FTransform(src + VP8Scan[16 + n], ref + VP8Scan[16 + n], tmp[n]);
  }
  if (DO_TRELLIS_UV && it->do_trellis_) {
    int ch, x, y;
    for (ch = 0, n = 0; ch <= 2; ch += 2) {
      for (y = 0; y < 2; ++y) {
        for (x = 0; x < 2; ++x, ++n) {
          const int ctx = it->top_nz_[4 + ch + x] + it->left_nz_[4 + ch + y];
          const int non_zero =
            TrellisQuantizeBlock(it, tmp[n], rd->uv_levels[n], ctx, 2,
                                 &dqm->uv_, dqm->lambda_trellis_uv_);
          it->top_nz_[4 + ch + x] = it->left_nz_[4 + ch + y] = non_zero;
          nz |= non_zero << n;
        }
      }
    }
  } else {
    for (n = 0; n < 8; ++n) {
      nz |= VP8EncQuantizeBlock(tmp[n], rd->uv_levels[n], 0, &dqm->uv_) << n;
    }
  }

  for (n = 0; n < 8; n += 2) {
    VP8ITransform(ref + VP8Scan[16 + n], tmp[n], yuv_out + VP8Scan[16 + n], 1);
  }
  return (nz << 16);
}


static void SwapPtr(uint8_t** a, uint8_t** b) {
  uint8_t* const tmp = *a;
  *a = *b;
  *b = tmp;
}

static void SwapOut(VP8EncIterator* const it) {
  SwapPtr(&it->yuv_out_, &it->yuv_out2_);
}

static void PickBestIntra16(VP8EncIterator* const it, VP8ModeScore* const rd) {
  const VP8Encoder* const enc = it->enc_;
  const VP8SegmentInfo* const dqm = &enc->dqm_[it->mb_->segment_];
  const int lambda = dqm->lambda_i16_;
  const int tlambda = dqm->tlambda_;
  const uint8_t* const src = it->yuv_in_ + Y_OFF;
  VP8ModeScore rd16;
  int mode;

  rd->mode_i16 = -1;
  for (mode = 0; mode < NUM_PRED_MODES; ++mode) {
    uint8_t* const tmp_dst = it->yuv_out2_ + Y_OFF;  
    int nz;

    
    nz = ReconstructIntra16(it, &rd16, tmp_dst, mode);

    
    rd16.D = VP8SSE16x16(src, tmp_dst);
    rd16.SD = tlambda ? MULT_8B(tlambda, VP8TDisto16x16(src, tmp_dst, kWeightY))
            : 0;
    rd16.R = VP8GetCostLuma16(it, &rd16);
    rd16.R += VP8FixedCostsI16[mode];

    
    SetRDScore(lambda, &rd16);
    if (mode == 0 || rd16.score < rd->score) {
      CopyScore(rd, &rd16);
      rd->mode_i16 = mode;
      rd->nz = nz;
      memcpy(rd->y_ac_levels, rd16.y_ac_levels, sizeof(rd16.y_ac_levels));
      memcpy(rd->y_dc_levels, rd16.y_dc_levels, sizeof(rd16.y_dc_levels));
      SwapOut(it);
    }
  }
  SetRDScore(dqm->lambda_mode_, rd);   
  VP8SetIntra16Mode(it, rd->mode_i16);
}


static const uint16_t* GetCostModeI4(VP8EncIterator* const it,
                                     const uint8_t modes[16]) {
  const int preds_w = it->enc_->preds_w_;
  const int x = (it->i4_ & 3), y = it->i4_ >> 2;
  const int left = (x == 0) ? it->preds_[y * preds_w - 1] : modes[it->i4_ - 1];
  const int top = (y == 0) ? it->preds_[-preds_w + x] : modes[it->i4_ - 4];
  return VP8FixedCostsI4[top][left];
}

static int PickBestIntra4(VP8EncIterator* const it, VP8ModeScore* const rd) {
  const VP8Encoder* const enc = it->enc_;
  const VP8SegmentInfo* const dqm = &enc->dqm_[it->mb_->segment_];
  const int lambda = dqm->lambda_i4_;
  const int tlambda = dqm->tlambda_;
  const uint8_t* const src0 = it->yuv_in_ + Y_OFF;
  uint8_t* const best_blocks = it->yuv_out2_ + Y_OFF;
  int total_header_bits = 0;
  VP8ModeScore rd_best;

  if (enc->max_i4_header_bits_ == 0) {
    return 0;
  }

  InitScore(&rd_best);
  rd_best.score = 211;  
  VP8IteratorStartI4(it);
  do {
    VP8ModeScore rd_i4;
    int mode;
    int best_mode = -1;
    const uint8_t* const src = src0 + VP8Scan[it->i4_];
    const uint16_t* const mode_costs = GetCostModeI4(it, rd->modes_i4);
    uint8_t* best_block = best_blocks + VP8Scan[it->i4_];
    uint8_t* tmp_dst = it->yuv_p_ + I4TMP;    

    InitScore(&rd_i4);
    VP8MakeIntra4Preds(it);
    for (mode = 0; mode < NUM_BMODES; ++mode) {
      VP8ModeScore rd_tmp;
      int16_t tmp_levels[16];

      
      rd_tmp.nz =
          ReconstructIntra4(it, tmp_levels, src, tmp_dst, mode) << it->i4_;

      
      rd_tmp.D = VP8SSE4x4(src, tmp_dst);
      rd_tmp.SD =
          tlambda ? MULT_8B(tlambda, VP8TDisto4x4(src, tmp_dst, kWeightY))
                  : 0;
      rd_tmp.R = VP8GetCostLuma4(it, tmp_levels);
      rd_tmp.R += mode_costs[mode];

      SetRDScore(lambda, &rd_tmp);
      if (best_mode < 0 || rd_tmp.score < rd_i4.score) {
        CopyScore(&rd_i4, &rd_tmp);
        best_mode = mode;
        SwapPtr(&tmp_dst, &best_block);
        memcpy(rd_best.y_ac_levels[it->i4_], tmp_levels, sizeof(tmp_levels));
      }
    }
    SetRDScore(dqm->lambda_mode_, &rd_i4);
    AddScore(&rd_best, &rd_i4);
    total_header_bits += mode_costs[best_mode];
    if (rd_best.score >= rd->score ||
        total_header_bits > enc->max_i4_header_bits_) {
      return 0;
    }
    
    if (best_block != best_blocks + VP8Scan[it->i4_])
      VP8Copy4x4(best_block, best_blocks + VP8Scan[it->i4_]);
    rd->modes_i4[it->i4_] = best_mode;
    it->top_nz_[it->i4_ & 3] = it->left_nz_[it->i4_ >> 2] = (rd_i4.nz ? 1 : 0);
  } while (VP8IteratorRotateI4(it, best_blocks));

  
  CopyScore(rd, &rd_best);
  VP8SetIntra4Mode(it, rd->modes_i4);
  SwapOut(it);
  memcpy(rd->y_ac_levels, rd_best.y_ac_levels, sizeof(rd->y_ac_levels));
  return 1;   
}


static void PickBestUV(VP8EncIterator* const it, VP8ModeScore* const rd) {
  const VP8Encoder* const enc = it->enc_;
  const VP8SegmentInfo* const dqm = &enc->dqm_[it->mb_->segment_];
  const int lambda = dqm->lambda_uv_;
  const uint8_t* const src = it->yuv_in_ + U_OFF;
  uint8_t* const tmp_dst = it->yuv_out2_ + U_OFF;  
  uint8_t* const dst0 = it->yuv_out_ + U_OFF;
  VP8ModeScore rd_best;
  int mode;

  rd->mode_uv = -1;
  InitScore(&rd_best);
  for (mode = 0; mode < NUM_PRED_MODES; ++mode) {
    VP8ModeScore rd_uv;

    
    rd_uv.nz = ReconstructUV(it, &rd_uv, tmp_dst, mode);

    
    rd_uv.D  = VP8SSE16x8(src, tmp_dst);
    rd_uv.SD = 0;    
    rd_uv.R  = VP8GetCostUV(it, &rd_uv);
    rd_uv.R += VP8FixedCostsUV[mode];

    SetRDScore(lambda, &rd_uv);
    if (mode == 0 || rd_uv.score < rd_best.score) {
      CopyScore(&rd_best, &rd_uv);
      rd->mode_uv = mode;
      memcpy(rd->uv_levels, rd_uv.uv_levels, sizeof(rd->uv_levels));
      memcpy(dst0, tmp_dst, UV_SIZE);   
    }
  }
  VP8SetIntraUVMode(it, rd->mode_uv);
  AddScore(rd, &rd_best);
}


static void SimpleQuantize(VP8EncIterator* const it, VP8ModeScore* const rd) {
  const VP8Encoder* const enc = it->enc_;
  const int is_i16 = (it->mb_->type_ == 1);
  int nz = 0;

  if (is_i16) {
    nz = ReconstructIntra16(it, rd, it->yuv_out_ + Y_OFF, it->preds_[0]);
  } else {
    VP8IteratorStartI4(it);
    do {
      const int mode =
          it->preds_[(it->i4_ & 3) + (it->i4_ >> 2) * enc->preds_w_];
      const uint8_t* const src = it->yuv_in_ + Y_OFF + VP8Scan[it->i4_];
      uint8_t* const dst = it->yuv_out_ + Y_OFF + VP8Scan[it->i4_];
      VP8MakeIntra4Preds(it);
      nz |= ReconstructIntra4(it, rd->y_ac_levels[it->i4_],
                              src, dst, mode) << it->i4_;
    } while (VP8IteratorRotateI4(it, it->yuv_out_ + Y_OFF));
  }

  nz |= ReconstructUV(it, rd, it->yuv_out_ + U_OFF, it->mb_->uv_mode_);
  rd->nz = nz;
}

static void DistoRefine(VP8EncIterator* const it, int try_both_i4_i16) {
  const int is_i16 = (it->mb_->type_ == 1);
  score_t best_score = MAX_COST;

  if (try_both_i4_i16 || is_i16) {
    int mode;
    int best_mode = -1;
    for (mode = 0; mode < NUM_PRED_MODES; ++mode) {
      const uint8_t* const ref = it->yuv_p_ + VP8I16ModeOffsets[mode];
      const uint8_t* const src = it->yuv_in_ + Y_OFF;
      const score_t score = VP8SSE16x16(src, ref);
      if (score < best_score) {
        best_mode = mode;
        best_score = score;
      }
    }
    VP8SetIntra16Mode(it, best_mode);
  }
  if (try_both_i4_i16 || !is_i16) {
    uint8_t modes_i4[16];
    
    
    score_t score_i4 = (score_t)I4_PENALTY;

    VP8IteratorStartI4(it);
    do {
      int mode;
      int best_sub_mode = -1;
      score_t best_sub_score = MAX_COST;
      const uint8_t* const src = it->yuv_in_ + Y_OFF + VP8Scan[it->i4_];

      
      
      VP8MakeIntra4Preds(it);
      for (mode = 0; mode < NUM_BMODES; ++mode) {
        const uint8_t* const ref = it->yuv_p_ + VP8I4ModeOffsets[mode];
        const score_t score = VP8SSE4x4(src, ref);
        if (score < best_sub_score) {
          best_sub_mode = mode;
          best_sub_score = score;
        }
      }
      modes_i4[it->i4_] = best_sub_mode;
      score_i4 += best_sub_score;
      if (score_i4 >= best_score) break;
    } while (VP8IteratorRotateI4(it, it->yuv_in_ + Y_OFF));
    if (score_i4 < best_score) {
      VP8SetIntra4Mode(it, modes_i4);
    }
  }
}


int VP8Decimate(VP8EncIterator* const it, VP8ModeScore* const rd,
                VP8RDLevel rd_opt) {
  int is_skipped;
  const int method = it->enc_->method_;

  InitScore(rd);

  
  
  VP8MakeLuma16Preds(it);
  VP8MakeChroma8Preds(it);

  if (rd_opt > RD_OPT_NONE) {
    it->do_trellis_ = (rd_opt >= RD_OPT_TRELLIS_ALL);
    PickBestIntra16(it, rd);
    if (method >= 2) {
      PickBestIntra4(it, rd);
    }
    PickBestUV(it, rd);
    if (rd_opt == RD_OPT_TRELLIS) {   
      it->do_trellis_ = 1;
      SimpleQuantize(it, rd);
    }
  } else {
    
    
    DistoRefine(it, (method >= 2));
    SimpleQuantize(it, rd);
  }
  is_skipped = (rd->nz == 0);
  VP8SetSkip(it, is_skipped);
  return is_skipped;
}

#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif