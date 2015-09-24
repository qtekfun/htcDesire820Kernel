// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_VIEWS_LOGIN_DISPLAY_HOST_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_VIEWS_LOGIN_DISPLAY_HOST_H_
#pragma once

#include <string>

#include "chrome/browser/chromeos/login/background_view.h"
#include "chrome/browser/chromeos/login/base_login_display_host.h"
#include "ui/gfx/rect.h"

namespace chromeos {

class ViewsLoginDisplayHost : public chromeos::BaseLoginDisplayHost {
 public:
  explicit ViewsLoginDisplayHost(const gfx::Rect& background_bounds);
  virtual ~ViewsLoginDisplayHost();

  
  virtual LoginDisplay* CreateLoginDisplay(LoginDisplay::Delegate* delegate)
      const;
  virtual gfx::NativeWindow GetNativeWindow() const;
  virtual void SetOobeProgress(BackgroundView::LoginStep step);
  virtual void SetOobeProgressBarVisible(bool visible);
  virtual void SetShutdownButtonEnabled(bool enable);
  virtual void SetStatusAreaEnabled(bool enable);
  virtual void SetStatusAreaVisible(bool visible);
  virtual void ShowBackground();

 private:
  
  BackgroundView* background_view_;
  views::Widget* background_window_;

  DISALLOW_COPY_AND_ASSIGN(ViewsLoginDisplayHost);
};

}  

#endif  
