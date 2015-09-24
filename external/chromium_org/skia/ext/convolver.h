// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_CONVOLVER_H_
#define SKIA_EXT_CONVOLVER_H_

#include <cmath>
#include <vector>

#include "base/basictypes.h"
#include "base/cpu.h"
#include "third_party/skia/include/core/SkSize.h"
#include "third_party/skia/include/core/SkTypes.h"

#if defined(ARCH_CPU_X86_FAMILY) && !defined(OS_IOS)
#define SIMD_SSE2 1
#define SIMD_PADDING 8  
#endif

#if defined (ARCH_CPU_MIPS_FAMILY) && \
    defined(__mips_dsp) && (__mips_dsp_rev >= 2)
#define SIMD_MIPS_DSPR2 1
#endif
#if defined(__APPLE__)
#undef FloatToFixed
#undef FixedToFloat
#endif

namespace skia {

class ConvolutionFilter1D {
 public:
  typedef short Fixed;

  
  enum { kShiftBits = 14 };

  SK_API ConvolutionFilter1D();
  SK_API ~ConvolutionFilter1D();

  
  static Fixed FloatToFixed(float f) {
    return static_cast<Fixed>(f * (1 << kShiftBits));
  }
  static unsigned char FixedToChar(Fixed x) {
    return static_cast<unsigned char>(x >> kShiftBits);
  }
  static float FixedToFloat(Fixed x) {
    
    
    
    COMPILE_ASSERT(sizeof(Fixed) == 2, fixed_type_should_fit_in_float_mantissa);
    float raw = static_cast<float>(x);
    return ldexpf(raw, -kShiftBits);
  }

  
  int max_filter() const { return max_filter_; }

  
  
  int num_values() const { return static_cast<int>(filters_.size()); }

  
  
  
  
  
  
  
  
  
  
  
  
  SK_API void AddFilter(int filter_offset,
                        const float* filter_values,
                        int filter_length);

  
  void AddFilter(int filter_offset,
                 const Fixed* filter_values,
                 int filter_length);

  
  
  
  
  
  inline const Fixed* FilterForValue(int value_offset,
                                     int* filter_offset,
                                     int* filter_length) const {
    const FilterInstance& filter = filters_[value_offset];
    *filter_offset = filter.offset;
    *filter_length = filter.trimmed_length;
    if (filter.trimmed_length == 0) {
      return NULL;
    }
    return &filter_values_[filter.data_location];
  }

  
  
  
  
  
  
  
  
  SK_API const Fixed* GetSingleFilter(int* specified_filter_length,
                                      int* filter_offset,
                                      int* filter_length) const;

  inline void PaddingForSIMD() {
    
    
    
    
    
#ifdef SIMD_PADDING
    for (int i = 0; i < SIMD_PADDING; ++i)
      filter_values_.push_back(static_cast<Fixed>(0));
#endif
  }

 private:
  struct FilterInstance {
    
    int data_location;

    
    int offset;

    
    int trimmed_length;

    
    
    
    int length;
  };

  
  std::vector<FilterInstance> filters_;

  
  
  
  std::vector<Fixed> filter_values_;

  
  int max_filter_;
};

SK_API void BGRAConvolve2D(const unsigned char* source_data,
                           int source_byte_row_stride,
                           bool source_has_alpha,
                           const ConvolutionFilter1D& xfilter,
                           const ConvolutionFilter1D& yfilter,
                           int output_byte_row_stride,
                           unsigned char* output,
                           bool use_simd_if_possible);

SK_API void SingleChannelConvolveX1D(const unsigned char* source_data,
                                     int source_byte_row_stride,
                                     int input_channel_index,
                                     int input_channel_count,
                                     const ConvolutionFilter1D& filter,
                                     const SkISize& image_size,
                                     unsigned char* output,
                                     int output_byte_row_stride,
                                     int output_channel_index,
                                     int output_channel_count,
                                     bool absolute_values);

SK_API void SingleChannelConvolveY1D(const unsigned char* source_data,
                                     int source_byte_row_stride,
                                     int input_channel_index,
                                     int input_channel_count,
                                     const ConvolutionFilter1D& filter,
                                     const SkISize& image_size,
                                     unsigned char* output,
                                     int output_byte_row_stride,
                                     int output_channel_index,
                                     int output_channel_count,
                                     bool absolute_values);

SK_API void SetUpGaussianConvolutionKernel(ConvolutionFilter1D* filter,
                                           float kernel_sigma,
                                           bool derivative);

}  

#endif  
