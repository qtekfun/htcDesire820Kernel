// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_TABS_NATIVE_VIEW_PHOTOBOOTH_H_
#define CHROME_BROWSER_UI_VIEWS_TABS_NATIVE_VIEW_PHOTOBOOTH_H_

#include "base/compiler_specific.h"
#include "ui/gfx/native_widget_types.h"

namespace gfx {
class Canvas;
class Rect;
}
class NativeViewPhotobooth {
 public:
  
  
  
  static NativeViewPhotobooth* Create(gfx::NativeView initial_view);

  
  virtual ~NativeViewPhotobooth() {}

  
  virtual void Replace(gfx::NativeView new_view) = 0;

  
  
  virtual void PaintScreenshotIntoCanvas(gfx::Canvas* canvas,
                                         const gfx::Rect& target_bounds) = 0;
};

#endif  
