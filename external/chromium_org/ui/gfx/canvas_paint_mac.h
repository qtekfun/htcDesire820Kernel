
// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_CANVAS_PAINT_MAC_H_
#define UI_GFX_CANVAS_PAINT_MAC_H_

#include "skia/ext/platform_canvas.h"
#include "ui/gfx/canvas.h"

#import <Cocoa/Cocoa.h>

namespace gfx {

class GFX_EXPORT CanvasSkiaPaint : public Canvas {
 public:
  
  explicit CanvasSkiaPaint(NSRect dirtyRect);
  CanvasSkiaPaint(NSRect dirtyRect, bool opaque);
  virtual ~CanvasSkiaPaint();

  
  
  void set_composite_alpha(bool composite_alpha) {
    composite_alpha_ = composite_alpha;
  }

  
  
  bool is_empty() const {
    return NSIsEmptyRect(rectangle_);
  }

  const NSRect& rectangle() const {
    return rectangle_;
  }

 private:
  void Init(bool opaque);

  CGContext* context_;
  NSRect rectangle_;
  
  bool composite_alpha_;

  
  CanvasSkiaPaint(const CanvasSkiaPaint&);
  CanvasSkiaPaint& operator=(const CanvasSkiaPaint&);
};

}  


#endif  
