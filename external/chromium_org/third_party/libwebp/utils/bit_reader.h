// Copyright 2010 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#ifndef WEBP_UTILS_BIT_READER_H_
#define WEBP_UTILS_BIT_READER_H_

#include <assert.h>
#ifdef _MSC_VER
#include <stdlib.h>  
#endif
#include <string.h>  
#include "../webp/types.h"

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif



#if !defined(WEBP_REFERENCE_IMPLEMENTATION)

#define USE_RIGHT_JUSTIFY

#if defined(__i386__) || defined(_M_IX86)      
#define BITS 16
#elif defined(__x86_64__) || defined(_M_X64)   
#define BITS 56
#elif defined(__arm__) || defined(_M_ARM)      
#define BITS 24
#else                      
#define BITS 24
#endif

#else     

#define USE_RIGHT_JUSTIFY
#define BITS 8

#endif



#if (BITS > 32)
typedef uint64_t bit_t;
typedef uint64_t lbit_t;
#elif (BITS == 32)
typedef uint64_t bit_t;
typedef uint32_t lbit_t;
#elif (BITS == 24)
typedef uint32_t bit_t;
typedef uint32_t lbit_t;
#elif (BITS == 16)
typedef uint32_t bit_t;
typedef uint16_t lbit_t;
#else
typedef uint32_t bit_t;
typedef uint8_t lbit_t;
#endif

#ifndef USE_RIGHT_JUSTIFY
typedef bit_t range_t;     
#define MASK ((((bit_t)1) << (BITS)) - 1)
#else
typedef uint32_t range_t;  
#endif


typedef struct VP8BitReader VP8BitReader;
struct VP8BitReader {
  const uint8_t* buf_;        
  const uint8_t* buf_end_;    
  int eof_;                   

  
  range_t range_;            
  bit_t value_;              
  int bits_;                 
};

void VP8InitBitReader(VP8BitReader* const br,
                      const uint8_t* const start, const uint8_t* const end);

uint32_t VP8GetValue(VP8BitReader* const br, int num_bits);
static WEBP_INLINE uint32_t VP8Get(VP8BitReader* const br) {
  return VP8GetValue(br, 1);
}

int32_t VP8GetSignedValue(VP8BitReader* const br, int num_bits);

extern const uint8_t kVP8Log2Range[128];
extern const range_t kVP8NewRange[128];

void VP8LoadFinalBytes(VP8BitReader* const br);    

static WEBP_INLINE void VP8LoadNewBytes(VP8BitReader* const br) {
  assert(br != NULL && br->buf_ != NULL);
  
  if (br->buf_ + sizeof(lbit_t) <= br->buf_end_) {
    
    bit_t bits;
    lbit_t in_bits = *(lbit_t*)br->buf_;
    br->buf_ += (BITS) >> 3;
#if !defined(__BIG_ENDIAN__)
#if (BITS > 32)
#if defined(__GNUC__) && \
           (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
    bits = (bit_t)__builtin_bswap64(in_bits);
#elif defined(_MSC_VER)
    bits = (bit_t)_byteswap_uint64(in_bits);
#elif defined(__x86_64__)
    __asm__ volatile("bswapq %0" : "=r"(bits) : "0"(in_bits));
#else  
    bits = (bit_t)in_bits;
    bits = ((bits & 0xffffffff00000000ull) >> 32) |
           ((bits & 0x00000000ffffffffull) << 32);
    bits = ((bits & 0xffff0000ffff0000ull) >> 16) |
           ((bits & 0x0000ffff0000ffffull) << 16);
    bits = ((bits & 0xff00ff00ff00ff00ull) >> 8) |
           ((bits & 0x00ff00ff00ff00ffull) << 8);
#endif
    bits >>= 64 - BITS;
#elif (BITS >= 24)
#if defined(__i386__) || defined(__x86_64__)
    __asm__ volatile("bswap %k0" : "=r"(in_bits) : "0"(in_bits));
    bits = (bit_t)in_bits;   
#elif defined(_MSC_VER)
    bits = (bit_t)_byteswap_ulong(in_bits);
#else
    bits = (bit_t)(in_bits >> 24) | ((in_bits >> 8) & 0xff00)
         | ((in_bits << 8) & 0xff0000)  | (in_bits << 24);
#endif  
    bits >>= (32 - BITS);
#elif (BITS == 16)
    
    bits = (bit_t)(in_bits >> 8) | ((in_bits & 0xff) << 8);
#else   
    bits = (bit_t)in_bits;
#endif
#else    
    bits = (bit_t)in_bits;
    if (BITS != 8 * sizeof(bit_t)) bits >>= (8 * sizeof(bit_t) - BITS);
#endif
#ifndef USE_RIGHT_JUSTIFY
    br->value_ |= bits << (-br->bits_);
#else
    br->value_ = bits | (br->value_ << (BITS));
#endif
    br->bits_ += (BITS);
  } else {
    VP8LoadFinalBytes(br);    
  }
}

static WEBP_INLINE int VP8BitUpdate(VP8BitReader* const br, range_t split) {
  if (br->bits_ < 0) {  
    VP8LoadNewBytes(br);
  }
#ifndef USE_RIGHT_JUSTIFY
  split |= (MASK);
  if (br->value_ > split) {
    br->range_ -= split + 1;
    br->value_ -= split + 1;
    return 1;
  } else {
    br->range_ = split;
    return 0;
  }
#else
  {
    const int pos = br->bits_;
    const range_t value = (range_t)(br->value_ >> pos);
    if (value > split) {
      br->range_ -= split + 1;
      br->value_ -= (bit_t)(split + 1) << pos;
      return 1;
    } else {
      br->range_ = split;
      return 0;
    }
  }
#endif
}

static WEBP_INLINE void VP8Shift(VP8BitReader* const br) {
#ifndef USE_RIGHT_JUSTIFY
  
  const bit_t idx = br->range_ >> (BITS);
  const int shift = kVP8Log2Range[idx];
  br->range_ = kVP8NewRange[idx];
  br->value_ <<= shift;
  br->bits_ -= shift;
#else
  const int shift = kVP8Log2Range[br->range_];
  assert(br->range_ < (range_t)128);
  br->range_ = kVP8NewRange[br->range_];
  br->bits_ -= shift;
#endif
}
static WEBP_INLINE int VP8GetBit(VP8BitReader* const br, int prob) {
#ifndef USE_RIGHT_JUSTIFY
  
  
  const range_t split =
      (range_t)((uint32_t)(br->range_ >> (BITS)) * prob) << ((BITS) - 8);
  const int bit = VP8BitUpdate(br, split);
  if (br->range_ <= (((range_t)0x7e << (BITS)) | (MASK))) {
    VP8Shift(br);
  }
  return bit;
#else
  const range_t split = (br->range_ * prob) >> 8;
  const int bit = VP8BitUpdate(br, split);
  if (br->range_ <= (range_t)0x7e) {
    VP8Shift(br);
  }
  return bit;
#endif
}

static WEBP_INLINE int VP8GetSigned(VP8BitReader* const br, int v) {
  const range_t split = (br->range_ >> 1);
  const int bit = VP8BitUpdate(br, split);
  VP8Shift(br);
  return bit ? -v : v;
}



typedef uint64_t vp8l_val_t;  

typedef struct {
  vp8l_val_t     val_;        
  const uint8_t* buf_;        
  size_t         len_;        
  size_t         pos_;        
  int            bit_pos_;    
  int            eos_;        
  int            error_;      
} VP8LBitReader;

void VP8LInitBitReader(VP8LBitReader* const br,
                       const uint8_t* const start,
                       size_t length);

void VP8LBitReaderSetBuffer(VP8LBitReader* const br,
                            const uint8_t* const buffer, size_t length);

uint32_t VP8LReadBits(VP8LBitReader* const br, int n_bits);

static WEBP_INLINE uint32_t VP8LPrefetchBits(VP8LBitReader* const br) {
  return (uint32_t)(br->val_ >> br->bit_pos_);
}

static WEBP_INLINE void VP8LDiscardBits(VP8LBitReader* const br, int num_bits) {
  br->bit_pos_ += num_bits;
}

void VP8LFillBitWindow(VP8LBitReader* const br);

#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif

#endif  
