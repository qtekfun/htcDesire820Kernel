// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "./vp8enci.h"
#include "./cost.h"
#include "../utils/utils.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#define MAX_ITERS_K_MEANS  6


static void SmoothSegmentMap(VP8Encoder* const enc) {
  int n, x, y;
  const int w = enc->mb_w_;
  const int h = enc->mb_h_;
  const int majority_cnt_3_x_3_grid = 5;
  uint8_t* const tmp = (uint8_t*)WebPSafeMalloc((uint64_t)w * h, sizeof(*tmp));
  assert((uint64_t)(w * h) == (uint64_t)w * h);   

  if (tmp == NULL) return;
  for (y = 1; y < h - 1; ++y) {
    for (x = 1; x < w - 1; ++x) {
      int cnt[NUM_MB_SEGMENTS] = { 0 };
      const VP8MBInfo* const mb = &enc->mb_info_[x + w * y];
      int majority_seg = mb->segment_;
      
      cnt[mb[-w - 1].segment_]++;  
      cnt[mb[-w + 0].segment_]++;  
      cnt[mb[-w + 1].segment_]++;  
      cnt[mb[   - 1].segment_]++;  
      cnt[mb[   + 1].segment_]++;  
      cnt[mb[ w - 1].segment_]++;  
      cnt[mb[ w + 0].segment_]++;  
      cnt[mb[ w + 1].segment_]++;  
      for (n = 0; n < NUM_MB_SEGMENTS; ++n) {
        if (cnt[n] >= majority_cnt_3_x_3_grid) {
          majority_seg = n;
        }
      }
      tmp[x + y * w] = majority_seg;
    }
  }
  for (y = 1; y < h - 1; ++y) {
    for (x = 1; x < w - 1; ++x) {
      VP8MBInfo* const mb = &enc->mb_info_[x + w * y];
      mb->segment_ = tmp[x + y * w];
    }
  }
  free(tmp);
}


static WEBP_INLINE int clip(int v, int m, int M) {
  return (v < m) ? m : (v > M) ? M : v;
}

static void SetSegmentAlphas(VP8Encoder* const enc,
                             const int centers[NUM_MB_SEGMENTS],
                             int mid) {
  const int nb = enc->segment_hdr_.num_segments_;
  int min = centers[0], max = centers[0];
  int n;

  if (nb > 1) {
    for (n = 0; n < nb; ++n) {
      if (min > centers[n]) min = centers[n];
      if (max < centers[n]) max = centers[n];
    }
  }
  if (max == min) max = min + 1;
  assert(mid <= max && mid >= min);
  for (n = 0; n < nb; ++n) {
    const int alpha = 255 * (centers[n] - mid) / (max - min);
    const int beta = 255 * (centers[n] - min) / (max - min);
    enc->dqm_[n].alpha_ = clip(alpha, -127, 127);
    enc->dqm_[n].beta_ = clip(beta, 0, 255);
  }
}


#define MAX_ALPHA 255                
#define ALPHA_SCALE (2 * MAX_ALPHA)  
#define DEFAULT_ALPHA (-1)
#define IS_BETTER_ALPHA(alpha, best_alpha) ((alpha) > (best_alpha))

static int FinalAlphaValue(int alpha) {
  alpha = MAX_ALPHA - alpha;
  return clip(alpha, 0, MAX_ALPHA);
}

static int GetAlpha(const VP8Histogram* const histo) {
  int max_value = 0, last_non_zero = 1;
  int k;
  int alpha;
  for (k = 0; k <= MAX_COEFF_THRESH; ++k) {
    const int value = histo->distribution[k];
    if (value > 0) {
      if (value > max_value) max_value = value;
      last_non_zero = k;
    }
  }
  
  
  
  alpha = (max_value > 1) ? ALPHA_SCALE * last_non_zero / max_value : 0;
  return alpha;
}

static void MergeHistograms(const VP8Histogram* const in,
                            VP8Histogram* const out) {
  int i;
  for (i = 0; i <= MAX_COEFF_THRESH; ++i) {
    out->distribution[i] += in->distribution[i];
  }
}


static void AssignSegments(VP8Encoder* const enc,
                           const int alphas[MAX_ALPHA + 1]) {
  const int nb = enc->segment_hdr_.num_segments_;
  int centers[NUM_MB_SEGMENTS];
  int weighted_average = 0;
  int map[MAX_ALPHA + 1];
  int a, n, k;
  int min_a = 0, max_a = MAX_ALPHA, range_a;
  
  int accum[NUM_MB_SEGMENTS], dist_accum[NUM_MB_SEGMENTS];

  
  for (n = 0; n <= MAX_ALPHA && alphas[n] == 0; ++n) {}
  min_a = n;
  for (n = MAX_ALPHA; n > min_a && alphas[n] == 0; --n) {}
  max_a = n;
  range_a = max_a - min_a;

  
  for (n = 1, k = 0; n < 2 * nb; n += 2) {
    centers[k++] = min_a + (n * range_a) / (2 * nb);
  }

  for (k = 0; k < MAX_ITERS_K_MEANS; ++k) {     
    int total_weight;
    int displaced;
    
    for (n = 0; n < nb; ++n) {
      accum[n] = 0;
      dist_accum[n] = 0;
    }
    
    n = 0;    
    for (a = min_a; a <= max_a; ++a) {
      if (alphas[a]) {
        while (n < nb - 1 && abs(a - centers[n + 1]) < abs(a - centers[n])) {
          n++;
        }
        map[a] = n;
        
        dist_accum[n] += a * alphas[a];
        accum[n] += alphas[a];
      }
    }
    
    
    displaced = 0;
    weighted_average = 0;
    total_weight = 0;
    for (n = 0; n < nb; ++n) {
      if (accum[n]) {
        const int new_center = (dist_accum[n] + accum[n] / 2) / accum[n];
        displaced += abs(centers[n] - new_center);
        centers[n] = new_center;
        weighted_average += new_center * accum[n];
        total_weight += accum[n];
      }
    }
    weighted_average = (weighted_average + total_weight / 2) / total_weight;
    if (displaced < 5) break;   
  }

  
  for (n = 0; n < enc->mb_w_ * enc->mb_h_; ++n) {
    VP8MBInfo* const mb = &enc->mb_info_[n];
    const int alpha = mb->alpha_;
    mb->segment_ = map[alpha];
    mb->alpha_ = centers[map[alpha]];  
  }

  if (nb > 1) {
    const int smooth = (enc->config_->preprocessing & 1);
    if (smooth) SmoothSegmentMap(enc);
  }

  SetSegmentAlphas(enc, centers, weighted_average);  
}


#define FAST_ANALYSIS_METHOD 4  
#define MAX_INTRA16_MODE 2
#define MAX_INTRA4_MODE  2
#define MAX_UV_MODE      2

static int MBAnalyzeBestIntra16Mode(VP8EncIterator* const it) {
  const int max_mode =
      (it->enc_->method_ >= FAST_ANALYSIS_METHOD) ? MAX_INTRA16_MODE
                                                  : NUM_PRED_MODES;
  int mode;
  int best_alpha = DEFAULT_ALPHA;
  int best_mode = 0;

  VP8MakeLuma16Preds(it);
  for (mode = 0; mode < max_mode; ++mode) {
    VP8Histogram histo = { { 0 } };
    int alpha;

    VP8CollectHistogram(it->yuv_in_ + Y_OFF,
                        it->yuv_p_ + VP8I16ModeOffsets[mode],
                        0, 16, &histo);
    alpha = GetAlpha(&histo);
    if (IS_BETTER_ALPHA(alpha, best_alpha)) {
      best_alpha = alpha;
      best_mode = mode;
    }
  }
  VP8SetIntra16Mode(it, best_mode);
  return best_alpha;
}

static int MBAnalyzeBestIntra4Mode(VP8EncIterator* const it,
                                   int best_alpha) {
  uint8_t modes[16];
  const int max_mode =
      (it->enc_->method_ >= FAST_ANALYSIS_METHOD) ? MAX_INTRA4_MODE
                                                  : NUM_BMODES;
  int i4_alpha;
  VP8Histogram total_histo = { { 0 } };
  int cur_histo = 0;

  VP8IteratorStartI4(it);
  do {
    int mode;
    int best_mode_alpha = DEFAULT_ALPHA;
    VP8Histogram histos[2];
    const uint8_t* const src = it->yuv_in_ + Y_OFF + VP8Scan[it->i4_];

    VP8MakeIntra4Preds(it);
    for (mode = 0; mode < max_mode; ++mode) {
      int alpha;

      memset(&histos[cur_histo], 0, sizeof(histos[cur_histo]));
      VP8CollectHistogram(src, it->yuv_p_ + VP8I4ModeOffsets[mode],
                          0, 1, &histos[cur_histo]);
      alpha = GetAlpha(&histos[cur_histo]);
      if (IS_BETTER_ALPHA(alpha, best_mode_alpha)) {
        best_mode_alpha = alpha;
        modes[it->i4_] = mode;
        cur_histo ^= 1;   
      }
    }
    
    MergeHistograms(&histos[cur_histo ^ 1], &total_histo);
    
  } while (VP8IteratorRotateI4(it, it->yuv_in_ + Y_OFF));

  i4_alpha = GetAlpha(&total_histo);
  if (IS_BETTER_ALPHA(i4_alpha, best_alpha)) {
    VP8SetIntra4Mode(it, modes);
    best_alpha = i4_alpha;
  }
  return best_alpha;
}

static int MBAnalyzeBestUVMode(VP8EncIterator* const it) {
  int best_alpha = DEFAULT_ALPHA;
  int best_mode = 0;
  const int max_mode =
      (it->enc_->method_ >= FAST_ANALYSIS_METHOD) ? MAX_UV_MODE
                                                  : NUM_PRED_MODES;
  int mode;
  VP8MakeChroma8Preds(it);
  for (mode = 0; mode < max_mode; ++mode) {
    VP8Histogram histo = { { 0 } };
    int alpha;
    VP8CollectHistogram(it->yuv_in_ + U_OFF,
                        it->yuv_p_ + VP8UVModeOffsets[mode],
                        16, 16 + 4 + 4, &histo);
    alpha = GetAlpha(&histo);
    if (IS_BETTER_ALPHA(alpha, best_alpha)) {
      best_alpha = alpha;
      best_mode = mode;
    }
  }
  VP8SetIntraUVMode(it, best_mode);
  return best_alpha;
}

static void MBAnalyze(VP8EncIterator* const it,
                      int alphas[MAX_ALPHA + 1],
                      int* const alpha, int* const uv_alpha) {
  const VP8Encoder* const enc = it->enc_;
  int best_alpha, best_uv_alpha;

  VP8SetIntra16Mode(it, 0);  
  VP8SetSkip(it, 0);         
  VP8SetSegment(it, 0);      

  best_alpha = MBAnalyzeBestIntra16Mode(it);
  if (enc->method_ >= 5) {
    
    
    
    
    best_alpha = MBAnalyzeBestIntra4Mode(it, best_alpha);
  }
  best_uv_alpha = MBAnalyzeBestUVMode(it);

  
  best_alpha = (3 * best_alpha + best_uv_alpha + 2) >> 2;
  best_alpha = FinalAlphaValue(best_alpha);
  alphas[best_alpha]++;
  it->mb_->alpha_ = best_alpha;   

  
  *alpha += best_alpha;   
  *uv_alpha += best_uv_alpha;
}

static void DefaultMBInfo(VP8MBInfo* const mb) {
  mb->type_ = 1;     
  mb->uv_mode_ = 0;
  mb->skip_ = 0;     
  mb->segment_ = 0;  
  mb->alpha_ = 0;
}


static void ResetAllMBInfo(VP8Encoder* const enc) {
  int n;
  for (n = 0; n < enc->mb_w_ * enc->mb_h_; ++n) {
    DefaultMBInfo(&enc->mb_info_[n]);
  }
  
  enc->dqm_[0].alpha_ = 0;
  enc->dqm_[0].beta_ = 0;
  
  WebPReportProgress(enc->pic_, enc->percent_ + 20, &enc->percent_);
}

int VP8EncAnalyze(VP8Encoder* const enc) {
  int ok = 1;
  const int do_segments =
      enc->config_->emulate_jpeg_size ||   
      (enc->segment_hdr_.num_segments_ > 1) ||
      (enc->method_ == 0);  
  enc->alpha_ = 0;
  enc->uv_alpha_ = 0;
  if (do_segments) {
    int alphas[MAX_ALPHA + 1] = { 0 };
    VP8EncIterator it;

    VP8IteratorInit(enc, &it);
    do {
      VP8IteratorImport(&it);
      MBAnalyze(&it, alphas, &enc->alpha_, &enc->uv_alpha_);
      ok = VP8IteratorProgress(&it, 20);
      
    } while (ok && VP8IteratorNext(&it, it.yuv_in_));
    enc->alpha_ /= enc->mb_w_ * enc->mb_h_;
    enc->uv_alpha_ /= enc->mb_w_ * enc->mb_h_;
    if (ok) AssignSegments(enc, alphas);
  } else {   
    ResetAllMBInfo(enc);
  }
  return ok;
}

#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif
