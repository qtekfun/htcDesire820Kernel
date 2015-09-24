// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_CANVAS_PAINT_H_
#define UI_GFX_CANVAS_PAINT_H_

#include "ui/gfx/gfx_export.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {

class Canvas;
class Rect;

class CanvasPaint {
 public:
  
  
  GFX_EXPORT static CanvasPaint* CreateCanvasPaint(gfx::NativeView view);

  virtual ~CanvasPaint() {}

  
  virtual bool IsValid() const = 0;

  
  virtual gfx::Rect GetInvalidRect() const = 0;

  
  virtual Canvas* AsCanvas() = 0;
};

}  

#endif  
