// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_SHADOW_VALUE_H_
#define UI_GFX_SHADOW_VALUE_H_

#include <string>
#include <vector>

#include "third_party/skia/include/core/SkColor.h"
#include "ui/gfx/gfx_export.h"
#include "ui/gfx/point.h"

namespace gfx {

class Insets;

class ShadowValue;
typedef std::vector<ShadowValue> ShadowValues;

class GFX_EXPORT ShadowValue {
 public:
  ShadowValue();
  ShadowValue(const gfx::Point& offset, double blur, SkColor color);
  ~ShadowValue();

  int x() const { return offset_.x(); }
  int y() const { return offset_.y(); }
  const gfx::Point& offset() const { return offset_; }
  double blur() const { return blur_; }
  SkColor color() const { return color_; }

  ShadowValue Scale(float scale) const;

  std::string ToString() const;

  
  
  static Insets GetMargin(const ShadowValues& shadows);

 private:
  gfx::Point offset_;

  
  
  
  
  
  
  
  
  double blur_;

  SkColor color_;
};

}  

#endif  
