// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_TEST_PIXEL_COMPARATOR_H_
#define CC_TEST_PIXEL_COMPARATOR_H_

#include "base/compiler_specific.h"
#include "third_party/skia/include/core/SkBitmap.h"

namespace cc {

class PixelComparator {
 public:
  virtual ~PixelComparator() {}

  virtual bool Compare(const SkBitmap& actual_bmp,
                       const SkBitmap& expected_bmp) const = 0;
};

class ExactPixelComparator : public PixelComparator {
 public:
  explicit ExactPixelComparator(const bool discard_alpha);
  virtual ~ExactPixelComparator() {}

  
  
  
  virtual bool Compare(const SkBitmap& actual_bmp,
                       const SkBitmap& expected_bmp) const OVERRIDE;

 private:
  
  bool discard_alpha_;
};

class FuzzyPixelComparator : public PixelComparator {
 public:
  FuzzyPixelComparator(const bool discard_alpha,
                       const float error_pixels_percentage_limit,
                       const float small_error_pixels_percentage_limit,
                       const float avg_abs_error_limit,
                       const int max_abs_error_limit,
                       const int small_error_threshold);
  virtual ~FuzzyPixelComparator() {}

  
  
  
  virtual bool Compare(const SkBitmap& actual_bmp,
                       const SkBitmap& expected_bmp) const OVERRIDE;

 private:
  
  bool discard_alpha_;
  
  float error_pixels_percentage_limit_;
  
  float small_error_pixels_percentage_limit_;
  
  float avg_abs_error_limit_;
  
  int max_abs_error_limit_;
  
  int small_error_threshold_;
};

}  

#endif  
