// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_REMOTING_REMOTE_DESKTOP_BROWSERTEST_H_
#define CHROME_TEST_REMOTING_REMOTE_DESKTOP_BROWSERTEST_H_

#include "chrome/browser/apps/app_browsertest_util.h"
#include "chrome/browser/chrome_notification_types.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/browser/notification_service.h"
#include "content/public/test/browser_test_utils.h"
#include "net/dns/mock_host_resolver.h"

namespace {
const char kOverrideUserDataDir[] = "override-user-data-dir";
const char kNoCleanup[] = "no-cleanup";
const char kNoInstall[] = "no-install";
const char kWebAppCrx[] = "webapp-crx";
const char kWebAppUnpacked[] = "webapp-unpacked";
const char kUsername[] = "username";
const char kkPassword[] = "password";
const char kMe2MePin[] = "me2me-pin";
const char kRemoteHostName[] = "remote-host-name";

inline void _ASSERT_TRUE(bool condition) {
  ASSERT_TRUE(condition);
  return;
}

}  

using extensions::Extension;

namespace remoting {

class RemoteDesktopBrowserTest : public extensions::PlatformAppBrowserTest {
 public:
  RemoteDesktopBrowserTest();
  virtual ~RemoteDesktopBrowserTest();

  
  virtual void SetUp() OVERRIDE;
  virtual void SetUpOnMainThread() OVERRIDE;

 protected:
  
  virtual void SetUpInProcessBrowserTestFixture() OVERRIDE;

  
  virtual void TearDownInProcessBrowserTestFixture() OVERRIDE;

  

  
  void VerifyInternetAccess();

  
  void InstallChromotingAppCrx();

  
  void InstallChromotingAppUnpacked();

  
  void UninstallChromotingApp();

  
  void VerifyChromotingLoaded(bool expected);

  
  void LaunchChromotingApp();

  
  void Authorize();

  
  void Authenticate();

  
  void Approve();

  
  void ExpandMe2Me();

  
  void DisconnectMe2Me();

  
  void SimulateKeyPressWithCode(ui::KeyboardCode keyCode, const char* code);

  void SimulateKeyPressWithCode(ui::KeyboardCode keyCode,
                                const char* code,
                                bool control,
                                bool shift,
                                bool alt,
                                bool command);

  
  void SimulateCharInput(char c);

  
  void SimulateStringInput(const std::string& input);

  
  void SimulateMouseLeftClickAt(int x, int y);

  
  void SimulateMouseClickAt(
      int modifiers, blink::WebMouseEvent::Button button, int x, int y);

  

  
  void Install();

  
  void Cleanup();

  
  
  
  void Auth();

  
  void ConnectToLocalHost(bool remember_pin);

  
  void ConnectToRemoteHost(const std::string& host_name, bool remember_pin);

  
  void EnterPin(const std::string& name, bool remember_pin);

  
  std::string me2me_pin() { return me2me_pin_; }

  
  std::string remote_host_name() { return remote_host_name_; }

  
  
  void EnableDNSLookupForThisTest(
    net::RuleBasedHostResolverProc* host_resolver);

  
  void DisableDNSLookupForThisTest();

  void ParseCommandLine();

  

  
  base::FilePath WebAppCrxPath() { return webapp_crx_; }

  
  std::string ChromotingID() { return extension_->id(); }

  
  bool is_platform_app() {
    return extension_->GetType() == extensions::Manifest::TYPE_PLATFORM_APP;
  }

  
  bool is_unpacked() {
    return !webapp_unpacked_.empty();
  }

  
  content::WebContents* active_web_contents() {
    DCHECK(!web_contents_stack_.empty());
    return web_contents_stack_.back();
  }

  
  
  bool NoCleanup() { return no_cleanup_; }

  
  
  bool NoInstall() { return no_install_; }

  
  GURL Chromoting_Main_URL() {
    return GURL("chrome-extension://" + ChromotingID() + "/main.html");
  }

  
  GURL GetCurrentURL() {
    return active_web_contents()->GetURL();
  }

  

  
  void ExecuteScript(const std::string& script);

  
  
  void ExecuteScriptAndWaitForAnyPageLoad(const std::string& script);

  
  
  bool ExecuteScriptAndExtractBool(const std::string& script) {
    return ExecuteScriptAndExtractBool(active_web_contents(), script);
  }

  
  static bool ExecuteScriptAndExtractBool(content::WebContents* web_contents,
                                          const std::string& script);

  
  
  int ExecuteScriptAndExtractInt(const std::string& script) {
    return ExecuteScriptAndExtractInt(active_web_contents(), script);
  }

  
  static int ExecuteScriptAndExtractInt(content::WebContents* web_contents,
                                        const std::string& script);

  
  
  std::string ExecuteScriptAndExtractString(const std::string& script) {
    return ExecuteScriptAndExtractString(active_web_contents(), script);
  }

  
  static std::string ExecuteScriptAndExtractString(
      content::WebContents* web_contents, const std::string& script);

  
  
  bool HtmlElementExists(const std::string& name) {
    return ExecuteScriptAndExtractBool(
        "document.getElementById(\"" + name + "\") != null");
  }

  
  
  bool HtmlElementVisible(const std::string& name);

  
  void ClickOnControl(const std::string& name);

  
  void WaitForConnection();

  
  bool IsLocalHostReady();

  
  
  bool IsPinFormVisible();

  
  
  bool IsSessionConnected();

  
  
  bool IsAuthenticated() {
      return IsAuthenticatedInWindow(active_web_contents());
  }

  
  void DismissHostVersionWarningIfVisible();

  
  
  static bool IsAuthenticatedInWindow(content::WebContents* web_contents);

 private:
  

  
  
  
  scoped_ptr<net::ScopedDefaultHostResolverProc> mock_host_resolver_override_;

  
  
  
  
  
  
  
  
  
  std::vector<content::WebContents*> web_contents_stack_;

  bool no_cleanup_;
  bool no_install_;
  const Extension* extension_;
  base::FilePath webapp_crx_;
  base::FilePath webapp_unpacked_;
  std::string username_;
  std::string password_;
  std::string me2me_pin_;
  std::string remote_host_name_;
};

}  

#endif  
