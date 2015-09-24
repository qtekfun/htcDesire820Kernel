// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_FOCUSABLE_BORDER_H_
#define UI_VIEWS_CONTROLS_FOCUSABLE_BORDER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/views/border.h"
#include "ui/views/view.h"

namespace gfx {
class Canvas;
class Insets;
}

namespace views {

class VIEWS_EXPORT FocusableBorder : public Border {
 public:
  FocusableBorder();

  
  void SetInsets(int top, int left, int bottom, int right);

  
  void SetColor(SkColor color);
  
  void UseDefaultColor();

  
  virtual void Paint(const View& view, gfx::Canvas* canvas) OVERRIDE;
  virtual gfx::Insets GetInsets() const OVERRIDE;
  virtual gfx::Size GetMinimumSize() const OVERRIDE;

 private:
  gfx::Insets insets_;

  
  SkColor override_color_;

  
  
  bool use_default_color_;

  DISALLOW_COPY_AND_ASSIGN(FocusableBorder);
};

}  

#endif  
