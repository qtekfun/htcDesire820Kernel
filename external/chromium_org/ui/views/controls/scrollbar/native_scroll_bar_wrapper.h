// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_SCROLLBAR_NATIVE_SCROLL_BAR_WRAPPER_H_
#define UI_VIEWS_CONTROLS_SCROLLBAR_NATIVE_SCROLL_BAR_WRAPPER_H_

#include "ui/views/views_export.h"

namespace ui {
class NativeTheme;
}

namespace views {

class NativeScrollBar;
class View;

class VIEWS_EXPORT NativeScrollBarWrapper {
 public:
  virtual ~NativeScrollBarWrapper() {}

  
  
  virtual void Update(int viewport_size, int content_size, int current_pos) = 0;

  
  virtual View* GetView() = 0;

  
  virtual int GetPosition() const = 0;

  
  static NativeScrollBarWrapper* CreateWrapper(NativeScrollBar* button);

  
  static int GetVerticalScrollBarWidth(const ui::NativeTheme* theme);
  static int GetHorizontalScrollBarHeight(const ui::NativeTheme* theme);
};

}  

#endif  
