// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_SKBITMAP_OPERATIONS_H_
#define UI_GFX_SKBITMAP_OPERATIONS_H_

#include "base/gtest_prod_util.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/shadow_value.h"

namespace gfx {
class Point;
class Size;
}

class SkBitmap;

class GFX_EXPORT SkBitmapOperations {
 public:
  
  
  enum RotationAmount {
    ROTATION_90_CW,
    ROTATION_180_CW,
    ROTATION_270_CW,
  };

  
  
  
  static SkBitmap CreateInvertedBitmap(const SkBitmap& image);

  
  
  
  static SkBitmap CreateSuperimposedBitmap(const SkBitmap& first,
                                           const SkBitmap& second);

  
  
  
  static SkBitmap CreateBlendedBitmap(const SkBitmap& first,
                                      const SkBitmap& second,
                                      double alpha);

  
  
  
  static SkBitmap CreateMaskedBitmap(const SkBitmap& first,
                                     const SkBitmap& alpha);

  
  
  
  
  
  static SkBitmap CreateButtonBackground(SkColor color,
                                         const SkBitmap& image,
                                         const SkBitmap& mask);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static SkBitmap CreateHSLShiftedBitmap(const SkBitmap& bitmap,
                                         const color_utils::HSL& hsl_shift);

  
  
  
  static SkBitmap CreateTiledBitmap(const SkBitmap& bitmap,
                                    int src_x, int src_y,
                                    int dst_w, int dst_h);

  
  
  
  
  
  static SkBitmap DownsampleByTwoUntilSize(const SkBitmap& bitmap,
                                           int min_w, int min_h);

  
  
  static SkBitmap DownsampleByTwo(const SkBitmap& bitmap);

  
  
  
  
  static SkBitmap UnPreMultiply(const SkBitmap& bitmap);

  
  static SkBitmap CreateTransposedBitmap(const SkBitmap& bitmap);

  
  
  static SkBitmap CreateColorMask(const SkBitmap& bitmap, SkColor c);

  
  
  
  
  static SkBitmap CreateDropShadow(const SkBitmap& bitmap,
                                   const gfx::ShadowValues& shadows);

  
  static SkBitmap Rotate(const SkBitmap& source, RotationAmount rotation);

 private:
  SkBitmapOperations();  

  FRIEND_TEST_ALL_PREFIXES(SkBitmapOperationsTest, DownsampleByTwo);
  FRIEND_TEST_ALL_PREFIXES(SkBitmapOperationsTest, DownsampleByTwoSmall);
};

#endif  
