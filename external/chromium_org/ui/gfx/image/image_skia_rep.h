// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_IMAGE_IMAGE_SKIA_REP_H_
#define UI_GFX_IMAGE_IMAGE_SKIA_REP_H_

#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/size.h"

namespace gfx {

class GFX_EXPORT ImageSkiaRep {
 public:
  
  ImageSkiaRep();
  ~ImageSkiaRep();

  
  
  ImageSkiaRep(const gfx::Size& size, float scale);

  
  
  ImageSkiaRep(const SkBitmap& src, float scale);

  
  bool is_null() const { return bitmap_.isNull(); }

  
  int GetWidth() const;
  int GetHeight() const;

  
  int pixel_width() const { return bitmap_.width(); }
  int pixel_height() const { return bitmap_.height(); }
  Size pixel_size() const {
    return Size(pixel_width(), pixel_height());
  }

  
  float scale() const { return scale_; }

  
  const SkBitmap& sk_bitmap() const { return bitmap_; }

 private:
  friend class ImageSkia;
  SkBitmap& mutable_sk_bitmap() { return bitmap_; }

  SkBitmap bitmap_;
  float scale_;
};

}  

#endif  
