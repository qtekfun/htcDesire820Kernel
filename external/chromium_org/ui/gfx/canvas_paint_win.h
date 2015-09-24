// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_CANVAS_PAINT_WIN_H_
#define UI_GFX_CANVAS_PAINT_WIN_H_

#include "skia/ext/platform_canvas.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/size.h"
#include "ui/gfx/win/dpi.h"

namespace gfx {

class GFX_EXPORT CanvasSkiaPaint : public Canvas {
 public:
  
  CanvasSkiaPaint(HWND hwnd, HDC dc, const PAINTSTRUCT& ps);
  virtual ~CanvasSkiaPaint();

  
  
  CanvasSkiaPaint(HDC dc, bool opaque, int x, int y, int w, int h);

  
  virtual gfx::Rect GetInvalidRect() const;

  
  
  bool is_empty() const {
    return ps_.rcPaint.right - ps_.rcPaint.left == 0 ||
           ps_.rcPaint.bottom - ps_.rcPaint.top == 0;
  };

  
  
  const PAINTSTRUCT& paint_struct() const { return ps_; }

  
  HDC paint_dc() const { return paint_dc_; }

 private:
  void Init(bool opaque);

  HWND hwnd_;
  HDC paint_dc_;
  PAINTSTRUCT ps_;

  
  DISALLOW_COPY_AND_ASSIGN(CanvasSkiaPaint);
};

}  

#endif  
