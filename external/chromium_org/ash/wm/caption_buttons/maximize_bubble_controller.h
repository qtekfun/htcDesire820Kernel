// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_CAPTION_BUTTONS_MAXIMIZE_BUBBLE_CONTROLLER_H_
#define ASH_WM_CAPTION_BUTTONS_MAXIMIZE_BUBBLE_CONTROLLER_H_

#include "ash/ash_export.h"
#include "ash/wm/caption_buttons/caption_button_types.h"
#include "ash/wm/workspace/snap_types.h"
#include "base/memory/scoped_ptr.h"

namespace aura {
class Window;
}

namespace base {
class Timer;
}

namespace views {
class CustomButton;
}

namespace ash {

class FrameMaximizeButton;
class MaximizeBubbleControllerBubble;

class ASH_EXPORT MaximizeBubbleController {
 public:
  MaximizeBubbleController(FrameMaximizeButton* frame_maximize_button,
                           MaximizeBubbleFrameState maximize_type,
                           int appearance_delay_ms);
  
  
  virtual ~MaximizeBubbleController();

  
  void SetSnapType(SnapType snap_type);

  
  
  aura::Window* GetBubbleWindow();

  
  void DelayCreation();

  
  void OnButtonClicked(SnapType snap_type);

  
  
  
  void OnButtonHover(SnapType snap_type);

  
  FrameMaximizeButton* frame_maximize_button() {
    return frame_maximize_button_;
  }

  
  MaximizeBubbleFrameState maximize_type() const { return maximize_type_; }

  
  
  views::CustomButton* GetButtonForUnitTest(SnapType state);

  
  
  
  void RequestDestructionThroughOwner();

 private:
  
  void CreateBubble();

  
  FrameMaximizeButton* frame_maximize_button_;

  
  MaximizeBubbleControllerBubble* bubble_;

  
  const MaximizeBubbleFrameState maximize_type_;

  
  
  
  SnapType snap_type_for_creation_;

  
  scoped_ptr<base::Timer> timer_;

  
  const int appearance_delay_ms_;

  DISALLOW_COPY_AND_ASSIGN(MaximizeBubbleController);
};

}  

#endif  
