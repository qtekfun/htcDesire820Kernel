// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREEN_LOCKER_DELEGATE_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREEN_LOCKER_DELEGATE_H_

#include "base/callback_forward.h"
#include "base/strings/string16.h"
#include "chrome/browser/chromeos/login/help_app_launcher.h"
#include "ui/gfx/native_widget_types.h"

class GURL;

namespace content {
class WebUI;
}

namespace gfx {
class Image;
}

namespace chromeos {

class ScreenLocker;

class ScreenLockerDelegate {
 public:
  explicit ScreenLockerDelegate(ScreenLocker* screen_locker);
  virtual ~ScreenLockerDelegate();

  
  
  virtual void LockScreen() = 0;

  
  virtual void ScreenLockReady();

  
  
  virtual void OnAuthenticate() = 0;

  
  virtual void SetInputEnabled(bool enabled) = 0;

  
  virtual void ShowBannerMessage(const std::string& message) = 0;

  
  virtual void ShowUserPodButton(const std::string& username,
                                 const std::string& iconURL,
                                 const base::Closure& click_callback) = 0;

  
  
  
  virtual void ShowErrorMessage(
      int error_msg_id,
      HelpAppLauncher::HelpTopic help_topic_id) = 0;

  
  virtual void ClearErrors() = 0;

  
  
  virtual void AnimateAuthenticationSuccess() = 0;

  
  virtual gfx::NativeWindow GetNativeWindow() const = 0;

  
  
  virtual content::WebUI* GetAssociatedWebUI();

  
  virtual void OnLockWebUIReady() = 0;

  
  virtual void OnLockBackgroundDisplayed() = 0;

  
  ScreenLocker* screen_locker() { return screen_locker_; }

 protected:
  
  ScreenLocker* screen_locker_;

  DISALLOW_COPY_AND_ASSIGN(ScreenLockerDelegate);
};

}  

#endif  
