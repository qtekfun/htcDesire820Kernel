// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_ERROR_SCREEN_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_ERROR_SCREEN_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/chromeos/login/oobe_display.h"
#include "chrome/browser/chromeos/login/oobe_display.h"
#include "chrome/browser/chromeos/login/screens/wizard_screen.h"

namespace chromeos {

class ErrorScreenActor;
class ScreenObserver;

class ErrorScreen : public WizardScreen {
 public:
  enum UIState {
    UI_STATE_UNKNOWN = 0,
    UI_STATE_UPDATE,
    UI_STATE_SIGNIN,
    UI_STATE_LOCALLY_MANAGED,
    UI_STATE_KIOSK_MODE,
    UI_STATE_LOCAL_STATE_ERROR
  };

  enum ErrorState {
    ERROR_STATE_UNKNOWN = 0,
    ERROR_STATE_PORTAL,
    ERROR_STATE_OFFLINE,
    ERROR_STATE_PROXY,
    ERROR_STATE_AUTH_EXT_TIMEOUT
  };

  ErrorScreen(ScreenObserver* screen_observer, ErrorScreenActor* actor);
  virtual ~ErrorScreen();

  
  virtual void PrepareToShow() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual std::string GetName() const OVERRIDE;

  
  void FixCaptivePortal();

  
  void ShowCaptivePortal();

  
  void HideCaptivePortal();

  
  void SetUIState(UIState ui_state);

  UIState GetUIState() const;

  
  
  void SetErrorState(ErrorState error_state, const std::string& network);

  void set_parent_screen(OobeDisplay::Screen parent_screen) {
    parent_screen_ = parent_screen;
  }
  OobeDisplay::Screen parent_screen() const { return parent_screen_; }

 private:
  ErrorScreenActor* actor_;

  OobeDisplay::Screen parent_screen_;

  DISALLOW_COPY_AND_ASSIGN(ErrorScreen);
};

}  

#endif  
