// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GTK_INFOBARS_INFOBAR_ARROW_MODEL_H_
#define CHROME_BROWSER_UI_GTK_INFOBARS_INFOBAR_ARROW_MODEL_H_
#pragma once

#include <gtk/gtk.h>

#include "third_party/skia/include/core/SkPaint.h"
#include "ui/base/animation/animation_delegate.h"
#include "ui/base/animation/slide_animation.h"

namespace gfx {
class Rect;
}

class InfoBar;

class InfoBarArrowModel : public ui::AnimationDelegate {
 public:
  class Observer {
   public:
    
    virtual void PaintStateChanged() = 0;
  };

  explicit InfoBarArrowModel(Observer* observer);
  virtual ~InfoBarArrowModel();

  
  
  void ShowArrowFor(InfoBar* bar, bool animate);

  
  bool NeedToDrawInfoBarArrow();

  
  
  void Paint(GtkWidget* widget,
             GdkEventExpose* expose,
             const gfx::Rect& bounds,
             const GdkColor& border_color);

  
  virtual void AnimationEnded(const ui::Animation* animation);
  virtual void AnimationProgressed(const ui::Animation* animation);
  virtual void AnimationCanceled(const ui::Animation* animation);

  
  static const size_t kDefaultArrowSize;

 private:
  
  struct InfoBarColors {
    SkColor top;
    SkColor bottom;
  };

  
  
  InfoBarColors CurrentInfoBarColors();

  
  Observer* observer_;

  
  
  ui::SlideAnimation animation_;

  
  InfoBarColors target_colors_;
  
  InfoBarColors previous_colors_;

  DISALLOW_COPY_AND_ASSIGN(InfoBarArrowModel);
};

#endif  
