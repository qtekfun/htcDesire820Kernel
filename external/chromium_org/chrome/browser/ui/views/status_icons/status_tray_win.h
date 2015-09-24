// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_STATUS_ICONS_STATUS_TRAY_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_STATUS_ICONS_STATUS_TRAY_WIN_H_

#include <windows.h>

#include "base/compiler_specific.h"
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
  
  virtual StatusIcon* CreatePlatformStatusIcon(
      StatusIconType type,
      const gfx::ImageSkia& image,
      const base::string16& tool_tip) OVERRIDE;

 private:
  
  static LRESULT CALLBACK WndProcStatic(HWND hwnd,
                                        UINT message,
                                        WPARAM wparam,
                                        LPARAM lparam);

  UINT NextIconId();

  
  UINT next_icon_id_;

  
  ATOM atom_;

  
  HMODULE instance_;

  
  HWND window_;

  
  
  UINT taskbar_created_message_;

  DISALLOW_COPY_AND_ASSIGN(StatusTrayWin);
};

#endif  

