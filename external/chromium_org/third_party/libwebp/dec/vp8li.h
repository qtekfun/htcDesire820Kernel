// Copyright 2012 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#ifndef WEBP_DEC_VP8LI_H_
#define WEBP_DEC_VP8LI_H_

#include <string.h>     
#include "./webpi.h"
#include "../utils/bit_reader.h"
#include "../utils/color_cache.h"
#include "../utils/huffman.h"
#include "../webp/format_constants.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef enum {
  READ_DATA = 0,
  READ_HDR = 1,
  READ_DIM = 2
} VP8LDecodeState;

typedef struct VP8LTransform VP8LTransform;
struct VP8LTransform {
  VP8LImageTransformType type_;   
  int                    bits_;   
  int                    xsize_;  
  int                    ysize_;  
  uint32_t              *data_;   
};

typedef struct {
  HuffmanTree htrees_[HUFFMAN_CODES_PER_META_CODE];
} HTreeGroup;

typedef struct {
  int             color_cache_size_;
  VP8LColorCache  color_cache_;

  int             huffman_mask_;
  int             huffman_subsample_bits_;
  int             huffman_xsize_;
  uint32_t       *huffman_image_;
  int             num_htree_groups_;
  HTreeGroup     *htree_groups_;
} VP8LMetadata;

typedef struct {
  VP8StatusCode    status_;
  VP8LDecodeState  action_;
  VP8LDecodeState  state_;
  VP8Io           *io_;

  const WebPDecBuffer *output_;    

  uint32_t        *pixels_;        
                                   
  uint32_t        *argb_cache_;    

  VP8LBitReader    br_;

  int              width_;
  int              height_;
  int              last_row_;      
  int              last_out_row_;  

  VP8LMetadata     hdr_;

  int              next_transform_;
  VP8LTransform    transforms_[NUM_TRANSFORMS];
  
  uint32_t         transforms_seen_;

  uint8_t         *rescaler_memory;  
  WebPRescaler    *rescaler;         
} VP8LDecoder;



int VP8LDecodeAlphaImageStream(int width, int height, const uint8_t* const data,
                               size_t data_size, uint8_t* const output);

VP8LDecoder* VP8LNew(void);

int VP8LDecodeHeader(VP8LDecoder* const dec, VP8Io* const io);

int VP8LDecodeImage(VP8LDecoder* const dec);

void VP8LClear(VP8LDecoder* const dec);

void VP8LDelete(VP8LDecoder* const dec);


#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif

#endif  
