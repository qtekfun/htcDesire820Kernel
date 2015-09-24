// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_STATUS_ICONS_STATUS_ICON_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_STATUS_ICONS_STATUS_ICON_WIN_H_
#pragma once

#include <windows.h>
#include <shellapi.h>

#include "base/memory/scoped_ptr.h"
#include "base/win/scoped_gdi_object.h"
#include "chrome/browser/status_icons/status_icon.h"

namespace views {
class Menu2;
}

class StatusIconWin : public StatusIcon {
 public:
  
  StatusIconWin(UINT id, HWND window, UINT message);
  virtual ~StatusIconWin();

  
  virtual void SetImage(const SkBitmap& image);
  virtual void SetPressedImage(const SkBitmap& image);
  virtual void SetToolTip(const string16& tool_tip);
  virtual void DisplayBalloon(const string16& title, const string16& contents);

  UINT icon_id() const { return icon_id_; }

  UINT message_id() const { return message_id_; }

  
  
  
  void HandleClickEvent(int x, int y, bool left_button_click);

  
  void ResetIcon();

 protected:
  
  virtual void UpdatePlatformContextMenu(ui::MenuModel* menu);

 private:
  void InitIconData(NOTIFYICONDATA* icon_data);

  
  UINT icon_id_;

  
  HWND window_;

  
  UINT message_id_;

  
  base::win::ScopedHICON icon_;

  
  scoped_ptr<views::Menu2> context_menu_;

  DISALLOW_COPY_AND_ASSIGN(StatusIconWin);
};

#endif  
