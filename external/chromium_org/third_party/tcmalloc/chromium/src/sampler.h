// Copyright (c) 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_SAMPLER_H_
#define TCMALLOC_SAMPLER_H_

#include "config.h"
#include <stddef.h>                     
#ifdef HAVE_STDINT_H
#include <stdint.h>                     
#endif
#include <string.h>                     
#include "base/basictypes.h"  
#include "internal_logging.h"  

namespace tcmalloc {


class PERFTOOLS_DLL_DECL Sampler {
 public:
  
  
  
  void Init(uint32_t seed);
  void Cleanup();

  
  
  bool SampleAllocation(size_t k);

  
  size_t PickNextSamplingPoint();

  
  static void InitStatics();

  
  int GetSamplePeriod();

  
  static uint64_t NextRandom(uint64_t rnd_);  
  static double FastLog2(const double & d);  
  static void PopulateFastLog2Table();  

 private:
  size_t        bytes_until_sample_;    
  uint64_t      rnd_;                   

  
  
  
  static const int kFastlogNumBits = 10;
  static const int kFastlogMask = (1 << kFastlogNumBits) - 1;
  static double log_table_[1<<kFastlogNumBits];  
};

inline bool Sampler::SampleAllocation(size_t k) {
  if (bytes_until_sample_ < k) {
    bytes_until_sample_ = PickNextSamplingPoint();
    return true;
  } else {
    bytes_until_sample_ -= k;
    return false;
  }
}


inline uint64_t Sampler::NextRandom(uint64_t rnd) {
  const uint64_t prng_mult = 0x5DEECE66DLL;
  const uint64_t prng_add = 0xB;
  const uint64_t prng_mod_power = 48;
  const uint64_t prng_mod_mask =
                ~((~static_cast<uint64_t>(0)) << prng_mod_power);
  return (prng_mult * rnd + prng_add) & prng_mod_mask;
}

inline double Sampler::FastLog2(const double & d) {
  ASSERT(d>0);
  COMPILE_ASSERT(sizeof(d) == sizeof(uint64_t), DoubleMustBe64Bits);
  uint64_t x;
  memcpy(&x, &d, sizeof(x));   
  const uint32_t x_high = x >> 32;
  const uint32_t y = x_high >> (20 - kFastlogNumBits) & kFastlogMask;
  const int32_t exponent = ((x_high >> 20) & 0x7FF) - 1023;
  return exponent + log_table_[y];
}

}  

#endif  
