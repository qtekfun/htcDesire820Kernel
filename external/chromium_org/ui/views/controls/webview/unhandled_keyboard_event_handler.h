// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_CONTROLS_WEBVIEW_UNHANDLED_KEYBOARD_EVENT_HANDLER_H_
#define UI_VIEWS_CONTROLS_WEBVIEW_UNHANDLED_KEYBOARD_EVENT_HANDLER_H_

#include "base/basictypes.h"
#include "ui/views/controls/webview/webview_export.h"

namespace content {
struct NativeWebKeyboardEvent;
}

namespace views {
class FocusManager;

class WEBVIEW_EXPORT UnhandledKeyboardEventHandler {
 public:
  UnhandledKeyboardEventHandler();
  ~UnhandledKeyboardEventHandler();

  void HandleKeyboardEvent(const content::NativeWebKeyboardEvent& event,
                           FocusManager* focus_manager);

 private:
  
  
  
  
  
  
  bool ignore_next_char_event_;

  DISALLOW_COPY_AND_ASSIGN(UnhandledKeyboardEventHandler);
};

}  

#endif  
