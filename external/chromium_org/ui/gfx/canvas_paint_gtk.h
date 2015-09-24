
// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_CANVAS_PAINT_LINUX_H_
#define UI_GFX_CANVAS_PAINT_LINUX_H_

#include "base/logging.h"
#include "skia/ext/platform_canvas.h"
#include "ui/gfx/canvas.h"
#include <gdk/gdk.h>

namespace gfx {

class GFX_EXPORT CanvasSkiaPaint : public Canvas {
 public:
  
  explicit CanvasSkiaPaint(GdkEventExpose* event);
  CanvasSkiaPaint(GdkEventExpose* event, bool opaque);
  virtual ~CanvasSkiaPaint();

  
  
  
  void set_composite_alpha(bool composite_alpha) {
    composite_alpha_ = composite_alpha;
  }

  
  
  bool is_empty() const {
    return gdk_region_empty(region_);
  }

  GdkRectangle rectangle() const {
    GdkRectangle bounds;
    gdk_region_get_clipbox(region_, &bounds);
    return bounds;
  }

 private:
  void Init(bool opaque);

  cairo_t* context_;
  GdkWindow* window_;
  GdkRegion* region_;
  
  bool composite_alpha_;

  
  CanvasSkiaPaint(const CanvasSkiaPaint&);
  CanvasSkiaPaint& operator=(const CanvasSkiaPaint&);
};

class GFX_EXPORT CanvasSkiaPaintCairo : public Canvas {
 public:
  CanvasSkiaPaintCairo(cairo_t* cairo, Size size, bool opaque);
  virtual ~CanvasSkiaPaintCairo();

  
  
  
  void set_composite_alpha(bool composite_alpha) {
    composite_alpha_ = composite_alpha;
  }

  
  
  bool is_empty() const {
    return size_.IsEmpty();
  }

  Size size() const {
    return size_;
  }

 private:
  void Init(bool opaque);

  cairo_t* context_;
  cairo_t* dest_;
  Size size_;
  
  bool composite_alpha_;

  
  CanvasSkiaPaintCairo(const CanvasSkiaPaintCairo&);
  CanvasSkiaPaintCairo& operator=(const CanvasSkiaPaintCairo&);
};

}  

#endif  
