// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_MANAGER_TEST_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_LOGIN_MANAGER_TEST_H_

#include "chrome/browser/chromeos/login/mock_login_utils.h"
#include "chrome/browser/chromeos/login/test/js_checker.h"
#include "chrome/test/base/in_process_browser_test.h"

namespace content {
class WebContents;
}  

namespace chromeos {

class LoginManagerTest : public InProcessBrowserTest {
 public:
  explicit LoginManagerTest(bool should_launch_browser);

  
  virtual void CleanUpOnMainThread() OVERRIDE;
  virtual void SetUpCommandLine(CommandLine* command_line) OVERRIDE;
  virtual void SetUpInProcessBrowserTestFixture() OVERRIDE;
  virtual void SetUpOnMainThread() OVERRIDE;

  
  
  
  void RegisterUser(const std::string& username);

  
  void SetExpectedCredentials(const std::string& username,
                              const std::string& password);

  
  
  bool TryToLogin(const std::string& username, const std::string& password);

  
  
  
  bool AddUserTosession(const std::string& username,
                        const std::string& password);

  
  void LoginUser(const std::string& username);

  
  void AddUser(const std::string& username);

  
  
  void JSExpect(const std::string& expression);

  MockLoginUtils& login_utils() { return *mock_login_utils_; }

  content::WebContents* web_contents() { return web_contents_; }

 private:
  void InitializeWebContents();

  void set_web_contents(content::WebContents* web_contents) {
    web_contents_ = web_contents;
  }

  MockLoginUtils* mock_login_utils_;
  bool should_launch_browser_;
  content::WebContents* web_contents_;
  test::JSChecker js_checker_;

  DISALLOW_COPY_AND_ASSIGN(LoginManagerTest);
};

}  

#endif  
