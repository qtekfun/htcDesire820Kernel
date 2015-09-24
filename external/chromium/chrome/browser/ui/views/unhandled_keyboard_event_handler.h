// Copyright (c) 2006-2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_UNHANDLED_KEYBOARD_EVENT_HANDLER_H_
#define CHROME_BROWSER_UI_VIEWS_UNHANDLED_KEYBOARD_EVENT_HANDLER_H_
#pragma once

#include "views/view.h"
#include "content/common/native_web_keyboard_event.h"

namespace views {
class FocusManager;
}  

class UnhandledKeyboardEventHandler {
 public:
  UnhandledKeyboardEventHandler();
  ~UnhandledKeyboardEventHandler();

  void HandleKeyboardEvent(const NativeWebKeyboardEvent& event,
                           views::FocusManager* focus_manager);

 private:
  
  
  
  
  
  
  bool ignore_next_char_event_;

  DISALLOW_COPY_AND_ASSIGN(UnhandledKeyboardEventHandler);
};

#endif  
