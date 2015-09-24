// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_IN_PROCESS_BROWSER_TEST_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_IN_PROCESS_BROWSER_TEST_H_
#pragma once

#include <string>

#include "chrome/browser/chromeos/cros/cros_in_process_browser_test.h"

class Browser;
class Profile;
class WizardController;

namespace chromeos {

class LoginDisplayHost;

class WizardInProcessBrowserTest : public CrosInProcessBrowserTest {
 public:
  explicit WizardInProcessBrowserTest(const char* screen_name);

 protected:
  
  
  virtual void SetUpWizard() {}

  
  virtual Browser* CreateBrowser(Profile* profile);
  virtual void CleanUpOnMainThread();

  WizardController* controller() const { return controller_; }
  void set_controller(WizardController* controller) {
    controller_ = controller;
  }

 private:
  std::string screen_name_;
  WizardController* controller_;
  LoginDisplayHost* host_;

  DISALLOW_COPY_AND_ASSIGN(WizardInProcessBrowserTest);
};

}  

#endif  

