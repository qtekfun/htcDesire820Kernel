// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_STATUS_ICONS_STATUS_TRAY_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_STATUS_ICONS_STATUS_TRAY_WIN_H_
#pragma once

#include <windows.h>

#include "chrome/browser/status_icons/status_tray.h"

class StatusTrayWin : public StatusTray {
 public:
  StatusTrayWin();
  ~StatusTrayWin();

  
  LRESULT CALLBACK WndProc(HWND hwnd,
                           UINT message,
                           WPARAM wparam,
                           LPARAM lparam);
 protected:
  
  virtual StatusIcon* CreatePlatformStatusIcon();

 private:
  
  static LRESULT CALLBACK WndProcStatic(HWND hwnd,
                                        UINT message,
                                        WPARAM wparam,
                                        LPARAM lparam);

  
  UINT next_icon_id_;

  
  HWND window_;

  
  
  UINT taskbar_created_message_;

  DISALLOW_COPY_AND_ASSIGN(StatusTrayWin);
};

#endif  

