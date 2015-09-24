// Copyright 2012 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#ifndef WEBP_ENC_HISTOGRAM_H_
#define WEBP_ENC_HISTOGRAM_H_

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./backward_references.h"
#include "../webp/format_constants.h"
#include "../webp/types.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef struct {
  
  
  int literal_[PIX_OR_COPY_CODES_MAX];
  int red_[256];
  int blue_[256];
  int alpha_[256];
  
  int distance_[NUM_DISTANCE_CODES];
  int palette_code_bits_;
  double bit_cost_;   
} VP8LHistogram;

typedef struct {
  int size;         
  int max_size;     
  VP8LHistogram** histograms;
} VP8LHistogramSet;

void VP8LHistogramCreate(VP8LHistogram* const p,
                         const VP8LBackwardRefs* const refs,
                         int palette_code_bits);

void VP8LHistogramInit(VP8LHistogram* const p, int palette_code_bits);

void VP8LHistogramStoreRefs(const VP8LBackwardRefs* const refs,
                            VP8LHistogram* const histo);

VP8LHistogramSet* VP8LAllocateHistogramSet(int size, int cache_bits);

void VP8LHistogramAddSinglePixOrCopy(VP8LHistogram* const histo,
                                     const PixOrCopy* const v);

double VP8LHistogramEstimateBits(const VP8LHistogram* const p);

double VP8LHistogramEstimateBitsBulk(const VP8LHistogram* const p);

static WEBP_INLINE int VP8LHistogramNumCodes(const VP8LHistogram* const p) {
  return 256 + NUM_LENGTH_CODES +
      ((p->palette_code_bits_ > 0) ? (1 << p->palette_code_bits_) : 0);
}

int VP8LGetHistoImageSymbols(int xsize, int ysize,
                             const VP8LBackwardRefs* const refs,
                             int quality, int histogram_bits, int cache_bits,
                             VP8LHistogramSet* const image_in,
                             uint16_t* const histogram_symbols);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif  
