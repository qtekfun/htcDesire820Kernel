// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SKIA_EXT_RECURSIVE_GAUSSIAN_CONVOLUTION_H_
#define SKIA_EXT_RECURSIVE_GAUSSIAN_CONVOLUTION_H_

#include "skia/ext/convolver.h"
#include "third_party/skia/include/core/SkSize.h"
#include "third_party/skia/include/core/SkTypes.h"

namespace skia {

class RecursiveFilter {
 public:
  enum Order {
    FUNCTION,
    FIRST_DERIVATIVE,
    SECOND_DERIVATIVE
  };

  static float qFromSigma(float sigma);
  static void computeCoefficients(float q, float b[4]);
  SK_API RecursiveFilter(float sigma, Order order);

  Order order() const { return order_; }
  const float* b() const { return b_; }

 private:
  Order order_;
  float q_;
  float b_[4];
};

SK_API unsigned char SingleChannelRecursiveGaussianX(
    const unsigned char* source_data,
    int source_byte_row_stride,
    int input_channel_index,
    int input_channel_count,
    const RecursiveFilter& filter,
    const SkISize& image_size,
    unsigned char* output,
    int output_byte_row_stride,
    int output_channel_index,
    int output_channel_count,
    bool absolute_values);

SK_API unsigned char  SingleChannelRecursiveGaussianY(
    const unsigned char* source_data,
    int source_byte_row_stride,
    int input_channel_index,
    int input_channel_count,
    const RecursiveFilter& filter,
    const SkISize& image_size,
    unsigned char* output,
    int output_byte_row_stride,
    int output_channel_index,
    int output_channel_count,
    bool absolute_values);
}  

#endif  
