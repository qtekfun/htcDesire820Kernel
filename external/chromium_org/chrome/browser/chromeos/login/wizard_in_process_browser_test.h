// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_IN_PROCESS_BROWSER_TEST_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_WIZARD_IN_PROCESS_BROWSER_TEST_H_

#include <string>

#include "base/compiler_specific.h"
#include "chrome/test/base/in_process_browser_test.h"

class Browser;
class Profile;

namespace chromeos {

class LoginDisplayHost;
class WizardController;

class WizardInProcessBrowserTest : public InProcessBrowserTest {
 public:
  explicit WizardInProcessBrowserTest(const char* screen_name);

  
  virtual void SetUp() OVERRIDE;

 protected:
  
  
  virtual void SetUpWizard() {}

  
  virtual void SetUpOnMainThread() OVERRIDE;
  virtual void SetUpCommandLine(CommandLine* command_line) OVERRIDE;
  virtual void CleanUpOnMainThread() OVERRIDE;

 private:
  std::string screen_name_;
  LoginDisplayHost* host_;

  DISALLOW_COPY_AND_ASSIGN(WizardInProcessBrowserTest);
};

}  

#endif  
