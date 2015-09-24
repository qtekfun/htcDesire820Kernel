// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#ifndef WEBP_UTILS_BIT_WRITER_H_
#define WEBP_UTILS_BIT_WRITER_H_

#include "../webp/types.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif


typedef struct VP8BitWriter VP8BitWriter;
struct VP8BitWriter {
  int32_t  range_;      
  int32_t  value_;
  int      run_;        
  int      nb_bits_;    
  uint8_t* buf_;        
  size_t   pos_;
  size_t   max_pos_;
  int      error_;      
};

int VP8BitWriterInit(VP8BitWriter* const bw, size_t expected_size);
uint8_t* VP8BitWriterFinish(VP8BitWriter* const bw);
void VP8BitWriterWipeOut(VP8BitWriter* const bw);

int VP8PutBit(VP8BitWriter* const bw, int bit, int prob);
int VP8PutBitUniform(VP8BitWriter* const bw, int bit);
void VP8PutValue(VP8BitWriter* const bw, int value, int nb_bits);
void VP8PutSignedValue(VP8BitWriter* const bw, int value, int nb_bits);

int VP8BitWriterAppend(VP8BitWriter* const bw,
                       const uint8_t* data, size_t size);

static WEBP_INLINE uint64_t VP8BitWriterPos(const VP8BitWriter* const bw) {
  return (uint64_t)(bw->pos_ + bw->run_) * 8 + 8 + bw->nb_bits_;
}

static WEBP_INLINE uint8_t* VP8BitWriterBuf(const VP8BitWriter* const bw) {
  return bw->buf_;
}
static WEBP_INLINE size_t VP8BitWriterSize(const VP8BitWriter* const bw) {
  return bw->pos_;
}


typedef struct {
  uint8_t* buf_;
  size_t bit_pos_;
  size_t max_bytes_;

  // After all bits are written, the caller must observe the state of
  
  
  
  int error_;
} VP8LBitWriter;

static WEBP_INLINE size_t VP8LBitWriterNumBytes(VP8LBitWriter* const bw) {
  return (bw->bit_pos_ + 7) >> 3;
}

static WEBP_INLINE uint8_t* VP8LBitWriterFinish(VP8LBitWriter* const bw) {
  return bw->buf_;
}

int VP8LBitWriterInit(VP8LBitWriter* const bw, size_t expected_size);

void VP8LBitWriterDestroy(VP8LBitWriter* const bw);

// Example: let's assume that 3 bits (Rs below) have been written already:
void VP8LWriteBits(VP8LBitWriter* const bw, int n_bits, uint32_t bits);


#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif

#endif  
