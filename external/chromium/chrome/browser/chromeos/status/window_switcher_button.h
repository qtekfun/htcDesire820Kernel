// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_STATUS_WINDOW_SWITCHER_BUTTON_H_
#define CHROME_BROWSER_CHROMEOS_STATUS_WINDOW_SWITCHER_BUTTON_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/status/status_area_button.h"
#include "chrome/browser/ui/browser_list.h"
#include "content/common/notification_observer.h"
#include "views/controls/button/menu_button.h"
#include "views/controls/menu/view_menu_delegate.h"

namespace chromeos {

class StatusAreaHost;

class WindowSwitcherButton : public StatusAreaButton,
                             public views::ViewMenuDelegate,
                             public BrowserList::Observer {
 public:
  explicit WindowSwitcherButton(StatusAreaHost* host);
  virtual ~WindowSwitcherButton();

 private:
  
  
  void UpdateStatus();

  
  virtual void OnBrowserAdded(const Browser* browser);
  virtual void OnBrowserRemoved(const Browser* browser);

  
  virtual void RunMenu(views::View* source, const gfx::Point& pt);

  DISALLOW_COPY_AND_ASSIGN(WindowSwitcherButton);
};

}  

#endif  
