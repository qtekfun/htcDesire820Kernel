// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_WIN_FOREGROUND_HELPER_H_
#define UI_BASE_WIN_FOREGROUND_HELPER_H_

#include "base/logging.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/win/window_impl.h"

namespace ui {

class UI_EXPORT ForegroundHelper : public gfx::WindowImpl {
 public:
  ForegroundHelper() : window_(NULL) { }

  BEGIN_MSG_MAP_EX(ForegroundHelper)
    MESSAGE_HANDLER(WM_HOTKEY, OnHotKey)
  END_MSG_MAP()

  
  
  
  static HRESULT SetForeground(HWND window);

 private:
  HRESULT ForegroundHotKey(HWND window);

  
  LRESULT OnHotKey(UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);

  HWND window_;

  DISALLOW_COPY_AND_ASSIGN(ForegroundHelper);
};

}  

#endif  
