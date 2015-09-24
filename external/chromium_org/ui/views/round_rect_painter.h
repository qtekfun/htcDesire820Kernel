// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_ROUNDED_RECT_PAINTER_H_
#define UI_VIEWS_ROUNDED_RECT_PAINTER_H_

#include "ui/gfx/rect.h"
#include "ui/views/painter.h"
#include "ui/views/views_export.h"

namespace gfx {
class Canvas;
class Path;
class Size;
}

namespace views {

class VIEWS_EXPORT RoundRectPainter : public Painter {
 public:
  RoundRectPainter(SkColor border_color, int corner_radius);
  virtual ~RoundRectPainter();

  
  virtual gfx::Size GetMinimumSize() const OVERRIDE;
  virtual void Paint(gfx::Canvas* canvas, const gfx::Size& size) OVERRIDE;

 private:
  const SkColor border_color_;
  const int corner_radius_;

  DISALLOW_COPY_AND_ASSIGN(RoundRectPainter);
};

}  

#endif  
