// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_WIDGET_CHILD_WINDOW_MESSAGE_PROCESSOR_H_
#define UI_VIEWS_WIDGET_CHILD_WINDOW_MESSAGE_PROCESSOR_H_

#include <windows.h>

namespace ui {
class ViewProp;
}

namespace views {

class ChildWindowMessageProcessor {
 public:
  
  
  static ui::ViewProp* Register(HWND hwnd,
                                 ChildWindowMessageProcessor* processor);

  
  
  static ChildWindowMessageProcessor* Get(HWND hwnd);

  
  
  
  
  virtual bool ProcessMessage(UINT message,
                              WPARAM w_param,
                              LPARAM l_param,
                              LRESULT* result) = 0;

 protected:
  virtual ~ChildWindowMessageProcessor() {}
};

}  

#endif  
