// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_DISPLAY_HOST_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_DISPLAY_HOST_H_
#pragma once

#include <string>

#include "chrome/browser/chromeos/login/background_view.h"
#include "chrome/browser/chromeos/customization_document.h"
#include "chrome/browser/chromeos/login/login_display.h"
#include "googleurl/src/gurl.h"
#include "ui/gfx/native_widget_types.h"

namespace chromeos {

class LoginDisplayHost {
 public:
  virtual ~LoginDisplayHost() {}

  
  virtual LoginDisplay* CreateLoginDisplay(
      LoginDisplay::Delegate* delegate) const = 0;

  
  
  virtual gfx::NativeWindow GetNativeWindow() const = 0;

  
  
  virtual void OnSessionStart() = 0;

  
  
  virtual void SetOobeProgress(BackgroundView::LoginStep step) = 0;

  
  virtual void SetOobeProgressBarVisible(bool visible) = 0;

  
  virtual void SetShutdownButtonEnabled(bool enable) = 0;

  
  virtual void SetStatusAreaEnabled(bool enable) = 0;

  
  virtual void SetStatusAreaVisible(bool visible) = 0;

  
  virtual void ShowBackground() = 0;

  
  
  
  virtual void StartWizard(
      const std::string& first_screen_name,
      const GURL& start_url) = 0;

  
  virtual void StartSignInScreen() = 0;
};

}  

#endif  
