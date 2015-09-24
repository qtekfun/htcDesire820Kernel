// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_CAPTION_BUTTONS_BUBBLE_CONTENTS_BUTTON_ROW_H_
#define ASH_WM_CAPTION_BUTTONS_BUBBLE_CONTENTS_BUTTON_ROW_H_

#include "ash/wm/workspace/snap_types.h"
#include "ui/views/controls/button/button.h"

namespace views {
class CustomButton;
}

namespace ash {

class BubbleDialogButton;
class MaximizeBubbleControllerBubble;

class BubbleContentsButtonRow : public views::View,
                                public views::ButtonListener {
 public:
  explicit BubbleContentsButtonRow(MaximizeBubbleControllerBubble* bubble);
  virtual ~BubbleContentsButtonRow();

  
  virtual void ButtonPressed(views::Button* sender,
                             const ui::Event& event) OVERRIDE;

  
  void ButtonHovered(BubbleDialogButton* sender);

  
  
  views::CustomButton* GetButtonForUnitTest(SnapType state);

  MaximizeBubbleControllerBubble* bubble() { return bubble_; }

 private:
  
  void AddMaximizeLeftButton();
  void AddMaximizeRightButton();
  void AddMinimizeButton();

  
  MaximizeBubbleControllerBubble* bubble_;

  
  BubbleDialogButton* left_button_;
  BubbleDialogButton* minimize_button_;
  BubbleDialogButton* right_button_;

  DISALLOW_COPY_AND_ASSIGN(BubbleContentsButtonRow);
};

}  

#endif  
