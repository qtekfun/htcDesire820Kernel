// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_WIN_MESSAGE_WINDOW_H_
#define BASE_WIN_MESSAGE_WINDOW_H_

#include <windows.h>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "base/threading/non_thread_safe.h"

namespace base {
namespace win {

class BASE_EXPORT MessageWindow : public base::NonThreadSafe {
 public:
  
  class WindowClass;

  
  
  
  typedef base::Callback<bool(UINT message,
                              WPARAM wparam,
                              LPARAM lparam,
                              LRESULT* result)> MessageCallback;

  MessageWindow();
  ~MessageWindow();

  
  
  bool Create(const MessageCallback& message_callback);

  
  bool CreateNamed(const MessageCallback& message_callback,
                   const string16& window_name);

  HWND hwnd() const { return window_; }

  
  
  static HWND FindWindow(const string16& window_name);

 private:
  
  friend class WindowClass;

  
  bool DoCreate(const MessageCallback& message_callback,
                const wchar_t* window_name);

  
  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam,
                                     LPARAM lparam);

  
  MessageCallback message_callback_;

  
  HWND window_;

  DISALLOW_COPY_AND_ASSIGN(MessageWindow);
};

}  
}  

#endif  
