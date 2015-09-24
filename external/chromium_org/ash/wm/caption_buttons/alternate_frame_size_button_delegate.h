// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_CAPTION_BUTTONS_ALTERNATE_FRAME_SIZE_BUTTON_DELEGATE_H_
#define ASH_WM_CAPTION_BUTTONS_ALTERNATE_FRAME_SIZE_BUTTON_DELEGATE_H_

#include "ash/ash_export.h"
#include "ash/wm/caption_buttons/caption_button_types.h"

namespace gfx {
class Insets;
class Point;
}

namespace ash {
class FrameCaptionButton;

class ASH_EXPORT AlternateFrameSizeButtonDelegate {
 public:
  enum Animate {
    ANIMATE_YES,
    ANIMATE_NO
  };

  
  virtual bool IsMinimizeButtonVisible() const = 0;

  
  
  
  virtual void SetButtonsToNormal(Animate animate) = 0;

  
  
  virtual void SetButtonIcons(CaptionButtonIcon left_button_action,
                              CaptionButtonIcon right_button_action,
                              Animate animate) = 0;

  
  
  
  
  
  
  virtual const FrameCaptionButton* PressButtonAt(
      const gfx::Point& position_in_screen,
      const gfx::Insets& pressed_button_hittest_insets) const = 0;

 protected:
  virtual ~AlternateFrameSizeButtonDelegate() {}
};

}  

#endif  
