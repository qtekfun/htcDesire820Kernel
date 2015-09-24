// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_BACKGROUND_H_
#define UI_VIEWS_BACKGROUND_H_

#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#endif  

#include "base/basictypes.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/views/views_export.h"

namespace gfx {
class Canvas;
}

namespace views {

class Painter;
class View;

class VIEWS_EXPORT Background {
 public:
  Background();
  virtual ~Background();

  
  static Background* CreateSolidBackground(SkColor color);

  
  static Background* CreateSolidBackground(int r, int g, int b) {
    return CreateSolidBackground(SkColorSetRGB(r, g, b));
  }

  
  static Background* CreateSolidBackground(int r, int g, int b, int a) {
    return CreateSolidBackground(SkColorSetARGB(a, r, g, b));
  }

  
  
  static Background* CreateVerticalGradientBackground(SkColor color1,
                                                      SkColor color2);

  
  
  
  
  
  static Background* CreateVerticalMultiColorGradientBackground(SkColor* colors,
                                                                SkScalar* pos,
                                                                size_t count);

  
  static Background* CreateStandardPanelBackground();

  
  
  static Background* CreateBackgroundPainter(bool owns_painter,
                                             Painter* painter);

  
  virtual void Paint(gfx::Canvas* canvas, View* view) const = 0;

  
  
  void SetNativeControlColor(SkColor color);

  
  
  
  
  SkColor get_color() const { return color_; }

#if defined(OS_WIN)
  

  
  HBRUSH GetNativeControlBrush() const;
#endif  

 private:
  SkColor color_;
#if defined(OS_WIN)
  
  mutable HBRUSH native_control_brush_;
#endif  

  DISALLOW_COPY_AND_ASSIGN(Background);
};

}  

#endif  
