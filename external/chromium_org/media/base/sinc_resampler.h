// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_SINC_RESAMPLER_H_
#define MEDIA_BASE_SINC_RESAMPLER_H_

#include "base/atomic_ref_count.h"
#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/aligned_memory.h"
#include "base/memory/scoped_ptr.h"
#include "build/build_config.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT SincResampler {
 public:
  enum {
    
    
    
    kKernelSize = 32,

    
    
    kDefaultRequestSize = 512,

    
    
    
    kKernelOffsetCount = 32,
    kKernelStorageSize = kKernelSize * (kKernelOffsetCount + 1),
  };

  
  
  static void InitializeCPUSpecificFeatures();

  
  
  
  typedef base::Callback<void(int frames, float* destination)> ReadCB;

  
  
  
  
  
  
  SincResampler(double io_sample_rate_ratio,
                int request_frames,
                const ReadCB& read_cb);
  virtual ~SincResampler();

  
  void Resample(int frames, float* destination);

  
  
  int ChunkSize() const;

  
  
  void Flush();

  
  
  
  void SetRatio(double io_sample_rate_ratio);

  float* get_kernel_for_testing() { return kernel_storage_.get(); }

 private:
  FRIEND_TEST_ALL_PREFIXES(SincResamplerTest, Convolve);
  FRIEND_TEST_ALL_PREFIXES(SincResamplerPerfTest, Convolve);

  void InitializeKernel();
  void UpdateRegions(bool second_load);

  
  
  
  
  static float Convolve_C(const float* input_ptr, const float* k1,
                          const float* k2, double kernel_interpolation_factor);
#if defined(ARCH_CPU_X86_FAMILY)
  static float Convolve_SSE(const float* input_ptr, const float* k1,
                            const float* k2,
                            double kernel_interpolation_factor);
#elif defined(ARCH_CPU_ARM_FAMILY) && defined(USE_NEON)
  static float Convolve_NEON(const float* input_ptr, const float* k1,
                             const float* k2,
                             double kernel_interpolation_factor);
#endif

  
  double io_sample_rate_ratio_;

  
  
  double virtual_source_idx_;

  
  bool buffer_primed_;

  
  const ReadCB read_cb_;

  
  const int request_frames_;

  
  int block_size_;

  
  const int input_buffer_size_;

  
  
  
  scoped_ptr<float[], base::ScopedPtrAlignedFree> kernel_storage_;
  scoped_ptr<float[], base::ScopedPtrAlignedFree> kernel_pre_sinc_storage_;
  scoped_ptr<float[], base::ScopedPtrAlignedFree> kernel_window_storage_;

  
  scoped_ptr<float[], base::ScopedPtrAlignedFree> input_buffer_;

  
  
  float* r0_;
  float* const r1_;
  float* const r2_;
  float* r3_;
  float* r4_;

  
  
  
  base::AtomicRefCount currently_resampling_;

  DISALLOW_COPY_AND_ASSIGN(SincResampler);
};

}  

#endif  
