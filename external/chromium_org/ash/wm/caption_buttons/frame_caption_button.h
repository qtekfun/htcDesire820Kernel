// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_CAPTION_BUTTONS_FRAME_CAPTION_BUTTON_H_
#define ASH_WM_CAPTION_BUTTONS_FRAME_CAPTION_BUTTON_H_

#include "ash/ash_export.h"
#include "ash/wm/caption_buttons/caption_button_types.h"
#include "base/memory/scoped_ptr.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/views/controls/button/image_button.h"

namespace gfx {
class SlideAnimation;
}

namespace ash {

class ASH_EXPORT FrameCaptionButton : public views::ImageButton {
 public:
  enum Animate {
    ANIMATE_YES,
    ANIMATE_NO
  };

  enum Style {
    
    STYLE_TALL_RESTORED,

    
    STYLE_SHORT_RESTORED,

    
    STYLE_SHORT_MAXIMIZED_OR_FULLSCREEN
  };

  static const char kViewClassName[];

  FrameCaptionButton(views::ButtonListener* listener, CaptionButtonIcon icon);
  virtual ~FrameCaptionButton();

  
  
  void SetIcon(CaptionButtonIcon icon, Animate animate);

  
  void SetStyle(Style style);

  
  virtual const char* GetClassName() const OVERRIDE;
  virtual void OnPaint(gfx::Canvas* canvas) OVERRIDE;

  CaptionButtonIcon icon() const {
    return icon_;
  }

 protected:
  
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;
  virtual void StateChanged() OVERRIDE;

 private:
  
  void UpdateImages();

  
  void SetImages(int normal_image_id, int hot_image_id, int pushed_image_id);

  
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

  
  CaptionButtonIcon icon_;

  
  Style style_;

  
  float last_paint_scale_;

  
  gfx::ImageSkia crossfade_image_;

  scoped_ptr<gfx::SlideAnimation> animation_;

  DISALLOW_COPY_AND_ASSIGN(FrameCaptionButton);
};

}  

#endif  
