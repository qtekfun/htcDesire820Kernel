// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_VIEWS_CACHED_LABEL_H_
#define UI_APP_LIST_VIEWS_CACHED_LABEL_H_

#include "ui/gfx/image/image_skia.h"
#include "ui/views/controls/label.h"
#include "ui/views/view.h"

namespace gfx {
class Canvas;
}

namespace app_list {

class CachedLabel : public views::Label {
 public:
  CachedLabel();

  
  void Invalidate() { needs_repaint_ = true; }

  
  void PaintToBackingImage();

#if defined(OS_WIN)
  
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;
#endif

 private:
  bool needs_repaint_;
  gfx::ImageSkia image_;
};

}  

#endif  
