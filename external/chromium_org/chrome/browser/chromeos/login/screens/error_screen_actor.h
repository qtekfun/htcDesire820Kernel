// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_ERROR_SCREEN_ACTOR_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_ERROR_SCREEN_ACTOR_H_

#include <string>

#include "base/basictypes.h"
#include "chrome/browser/chromeos/login/screens/error_screen.h"
#include "chrome/browser/ui/webui/chromeos/login/oobe_ui.h"

namespace base {
class DictionaryValue;
}

namespace chromeos {

class ErrorScreenActor {
 public:
  
  enum ErrorReason {
    ERROR_REASON_PROXY_AUTH_CANCELLED = 0,
    ERROR_REASON_PROXY_AUTH_SUPPLIED,
    ERROR_REASON_PROXY_CONNECTION_FAILED,
    ERROR_REASON_PROXY_CONFIG_CHANGED,
    ERROR_REASON_LOADING_TIMEOUT,
    ERROR_REASON_PORTAL_DETECTED,
    
    ERROR_REASON_NETWORK_STATE_CHANGED,
    
    ERROR_REASON_UPDATE,
    ERROR_REASON_FRAME_ERROR
  };

  ErrorScreenActor();
  virtual ~ErrorScreenActor();

  ErrorScreen::UIState ui_state() const { return ui_state_; }
  ErrorScreen::ErrorState error_state() const { return error_state_; }

  
  
  
  OobeUI::Screen parent_screen() const { return parent_screen_; }

  
  virtual void Show(OobeDisplay::Screen parent_screen,
                    base::DictionaryValue* params) = 0;

  
  virtual void Hide() = 0;

  
  virtual void FixCaptivePortal() = 0;

  
  virtual void ShowCaptivePortal() = 0;

  
  virtual void HideCaptivePortal() = 0;

  virtual void SetUIState(ErrorScreen::UIState ui_state) = 0;
  virtual void SetErrorState(ErrorScreen::ErrorState error_state,
                             const std::string& network) = 0;

  virtual void AllowGuestSignin(bool allowed) = 0;
  virtual void AllowOfflineLogin(bool allowed) = 0;

  static const char* ErrorReasonString(ErrorReason reason);

 protected:
  ErrorScreen::UIState ui_state_;
  ErrorScreen::ErrorState error_state_;
  std::string network_;
  bool guest_signin_allowed_;
  bool offline_login_allowed_;

  OobeUI::Screen parent_screen_;

  DISALLOW_COPY_AND_ASSIGN(ErrorScreenActor);
};

}  

#endif  
