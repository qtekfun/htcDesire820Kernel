// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_LINUX_UI_WINDOW_BUTTON_ORDER_OBSERVER_H_
#define UI_VIEWS_LINUX_UI_WINDOW_BUTTON_ORDER_OBSERVER_H_

#include <vector>

#include "ui/views/window/frame_buttons.h"

namespace views {

class WindowButtonOrderObserver {
 public:
  
  
  virtual void OnWindowButtonOrderingChange(
      const std::vector<views::FrameButton>& leading_buttons,
      const std::vector<views::FrameButton>& trailing_buttons) = 0;

 protected:
  virtual ~WindowButtonOrderObserver() {}
};

}  

#endif  
