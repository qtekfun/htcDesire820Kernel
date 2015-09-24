// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_IMAGE_SKIA_OPERATIONS_H_
#define UI_GFX_IMAGE_SKIA_OPERATIONS_H_

#include "base/gtest_prod_util.h"
#include "skia/ext/image_operations.h"
#include "ui/gfx/color_utils.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/shadow_value.h"
#include "ui/gfx/skbitmap_operations.h"

namespace gfx {
class ImageSkia;
class Rect;
class Size;

class GFX_EXPORT ImageSkiaOperations {
 public:
  
  
  
  static ImageSkia CreateBlendedImage(const ImageSkia& first,
                                      const ImageSkia& second,
                                      double alpha);

  
  static ImageSkia CreateTransparentImage(const ImageSkia& image, double alpha);

  
  
  static ImageSkia CreateSuperimposedImage(const ImageSkia& first,
                                           const ImageSkia& second);

  
  
  
  static ImageSkia CreateMaskedImage(const ImageSkia& first,
                                     const ImageSkia& alpha);

  
  
  
  static ImageSkia CreateTiledImage(const ImageSkia& image,
                                    int src_x, int src_y,
                                    int dst_w, int dst_h);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static ImageSkia CreateHSLShiftedImage(const gfx::ImageSkia& image,
                                         const color_utils::HSL& hsl_shift);

  
  
  
  
  
  static ImageSkia CreateButtonBackground(SkColor color,
                                          const gfx::ImageSkia& image,
                                          const gfx::ImageSkia& mask);

  
  
  static ImageSkia ExtractSubset(const gfx::ImageSkia& image,
                                 const gfx::Rect& subset_bounds);

  
  static ImageSkia CreateResizedImage(const ImageSkia& source,
                                      skia::ImageOperations::ResizeMethod methd,
                                      const Size& target_dip_size);

  
  static ImageSkia CreateImageWithDropShadow(const ImageSkia& source,
                                             const ShadowValues& shadows);

  
  
  static ImageSkia CreateRotatedImage(
      const ImageSkia& source,
      SkBitmapOperations::RotationAmount rotation);

 private:
  ImageSkiaOperations();  
};

}  

#endif  
