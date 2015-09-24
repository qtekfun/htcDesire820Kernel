// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_CAPTION_BUTTONS_ALTERNATE_FRAME_SIZE_BUTTON_H_
#define ASH_WM_CAPTION_BUTTONS_ALTERNATE_FRAME_SIZE_BUTTON_H_

#include "ash/ash_export.h"
#include "ash/wm/caption_buttons/alternate_frame_size_button_delegate.h"
#include "ash/wm/caption_buttons/frame_caption_button.h"
#include "ash/wm/workspace/snap_types.h"
#include "base/timer/timer.h"

namespace views {
class Widget;
}

namespace ash {
class AlternateFrameSizeButtonDelegate;

class ASH_EXPORT AlternateFrameSizeButton : public FrameCaptionButton {
 public:
  AlternateFrameSizeButton(views::ButtonListener* listener,
                           views::Widget* frame,
                           AlternateFrameSizeButtonDelegate* delegate);

  virtual ~AlternateFrameSizeButton();

  
  virtual bool OnMousePressed(const ui::MouseEvent& event) OVERRIDE;
  virtual bool OnMouseDragged(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseReleased(const ui::MouseEvent& event) OVERRIDE;
  virtual void OnMouseCaptureLost() OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  void set_delay_to_set_buttons_to_snap_mode(int delay_ms) {
    set_buttons_to_snap_mode_delay_ms_ = delay_ms;
  }

 private:
  
  void StartSetButtonsToSnapModeTimer(const ui::LocatedEvent& event);

  
  void SetButtonsToSnapMode();

  
  void UpdatePressedButton(const ui::LocatedEvent& event);

  
  
  bool CommitSnap(const ui::LocatedEvent& event);

  
  
  
  void SetButtonsToNormalMode(
      AlternateFrameSizeButtonDelegate::Animate animate);

  
  views::Widget* frame_;

  
  AlternateFrameSizeButtonDelegate* delegate_;

  
  
  gfx::Point set_buttons_to_snap_mode_timer_event_location_;

  
  
  
  int set_buttons_to_snap_mode_delay_ms_;

  base::OneShotTimer<AlternateFrameSizeButton> set_buttons_to_snap_mode_timer_;

  
  
  bool in_snap_mode_;

  
  
  SnapType snap_type_;

  DISALLOW_COPY_AND_ASSIGN(AlternateFrameSizeButton);
};

}  

#endif  
