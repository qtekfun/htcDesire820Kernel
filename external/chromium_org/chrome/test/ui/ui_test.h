// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_UI_UI_TEST_H_
#define CHROME_TEST_UI_UI_TEST_H_


#include <string>

#include "base/command_line.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/process/process.h"
#include "base/time/time.h"
#include "chrome/test/automation/proxy_launcher.h"
#include "testing/platform_test.h"
#include "url/gurl.h"

class AutomationProxy;
class BrowserProxy;
class GURL;
class TabProxy;

namespace base {
class DictionaryValue;
class FilePath;
}

class UITestBase {
 public:
  

  
  void LaunchBrowser();

  
  void LaunchBrowserAndServer();

  
  
  void ConnectToRunningBrowser();

  
  base::TimeDelta action_timeout();
  int action_timeout_ms();
  void set_action_timeout(base::TimeDelta timeout);
  void set_action_timeout_ms(int timeout);

  
  virtual ProxyLauncher* CreateProxyLauncher();

  
  void CloseBrowserAndServer();

  
  
  void LaunchBrowser(const CommandLine& cmdline, bool clear_profile);

  
  void QuitBrowser();

  
  
  
  void NavigateToURL(const GURL& url);

  
  void NavigateToURL(const GURL& url, int window_index);

  
  void NavigateToURL(const GURL& url, int window_index, int tab_index);

  
  
  
  
  void NavigateToURLBlockUntilNavigationsComplete(const GURL& url,
                                                  int number_of_navigations);

  
  void NavigateToURLBlockUntilNavigationsComplete(const GURL& url,
      int number_of_navigations, int tab_index, int window_index);

  
  
  
  GURL GetActiveTabURL() { return GetActiveTabURL(0); }

  
  GURL GetActiveTabURL(int window_index);

  
  
  std::wstring GetActiveTabTitle() { return GetActiveTabTitle(0); }

  
  std::wstring GetActiveTabTitle(int window_index);

  
  
  
  int GetActiveTabIndex() { return GetActiveTabIndex(0); }

  
  int GetActiveTabIndex(int window_index);

  
  
  int GetTabCount();

  
  int GetTabCount(int window_index);

  
  
  void WaitUntilTabCount(int tab_count);

  
  
  
  
  
  bool CloseBrowser(BrowserProxy* browser, bool* application_closed) const;

  
  const base::FilePath::CharType* GetExecutablePath();

  
  
  base::FilePath user_data_dir() const {
    return launcher_->user_data_dir();
  }

  
  
  
  
  
  const base::FilePath& template_user_data() const { return template_user_data_; }
  void set_template_user_data(const base::FilePath& template_user_data) {
    template_user_data_ = template_user_data;
  }

  
  
  
  base::ProcessHandle process() const { return launcher_->process(); }

  
  base::ProcessId browser_process_id() const { return launcher_->process_id(); }

  
  base::TimeTicks browser_launch_time() const {
    return launcher_->browser_launch_time();
  }

  
  base::TimeDelta browser_quit_time() const {
    return launcher_->browser_quit_time();
  }

  
  
  bool get_clear_profile() const {
    return clear_profile_;
  }
  
  
  void set_clear_profile(bool clear_profile) {
    clear_profile_ = clear_profile;
  }

  
  std::string homepage() {
    return homepage_;
  }

  
  
  void set_homepage(const std::string& homepage) {
    homepage_ = homepage;
  }

  void set_test_name(const std::string& name) {
    test_name_ = name;
  }

  
  void set_shutdown_type(ProxyLauncher::ShutdownType value) {
    launcher_->set_shutdown_type(value);
  }

  
  int GetCrashCount() const;

  
  
  
  std::string CheckErrorsAndCrashes() const;

  
  void SetBrowserDirectory(const base::FilePath& dir);

  
  
  void AppendBrowserLaunchSwitch(const char* name);

  
  
  void AppendBrowserLaunchSwitch(const char* name, const char* value);

  
  bool BeginTracing(const std::string& category_patterns);

  
  std::string EndTracing();

 protected:
  
  
  static const wchar_t kFailedNoCrashService[];

  UITestBase();
  explicit UITestBase(base::MessageLoop::Type msg_loop_type);

  virtual ~UITestBase();

  
  
  virtual void SetUp();

  
  virtual void TearDown();

  virtual AutomationProxy* automation() const;

  ProxyLauncher::LaunchState DefaultLaunchState();

  
  
  virtual void SetLaunchSwitches();

  
  
  virtual void SetUpProfile();

  
  
  
  
  scoped_refptr<TabProxy> GetActiveTab();

  
  scoped_refptr<TabProxy> GetActiveTab(int window_index);

  

  
  base::FilePath browser_directory_;

  
  base::FilePath test_data_directory_;

  
  CommandLine launch_arguments_;

  
  size_t expected_errors_;

  
  int expected_crashes_;

  
  std::string homepage_;

  
  std::string test_name_;

  
  bool wait_for_initial_loads_;

  
  bool dom_automation_enabled_;

  
  
  bool stats_collection_controller_enabled_;

  
  base::FilePath template_user_data_;

  
  bool show_window_;

  
  bool clear_profile_;

  
  
  bool include_testing_id_;

  
  bool enable_file_cookies_;

  
  scoped_ptr<ProxyLauncher> launcher_;

  
  base::FilePath websocket_pid_file_;

 private:
  
  base::Time test_start_time_;
};

class UITest : public UITestBase, public PlatformTest {
 protected:
  UITest() {}
  explicit UITest(base::MessageLoop::Type msg_loop_type)
      : UITestBase(), PlatformTest(), message_loop_(msg_loop_type) {}

  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  virtual ProxyLauncher* CreateProxyLauncher() OVERRIDE;

  
  
  bool GetBrowserProcessCount(int* count) WARN_UNUSED_RESULT;

  
  
  base::DictionaryValue* GetLocalState();

  
  
  
  base::DictionaryValue* GetDefaultProfilePreferences();

  
  
  void WaitForFinish(const std::string &name,
                     const std::string &id, const GURL &url,
                     const std::string& test_complete_cookie,
                     const std::string& expected_cookie_value,
                     const base::TimeDelta wait_time);

  
  
  
  
  
  
  
  
  bool WaitUntilJavaScriptCondition(TabProxy* tab,
                                    const std::wstring& frame_xpath,
                                    const std::wstring& jscript,
                                    base::TimeDelta timeout);

  
  
  
  
  
  bool WaitUntilCookieValue(TabProxy* tab, const GURL& url,
                            const char* cookie_name,
                            base::TimeDelta timeout,
                            const char* expected_value);

  
  
  
  
  
  std::string WaitUntilCookieNonEmpty(TabProxy* tab,
                                      const GURL& url,
                                      const char* cookie_name,
                                      base::TimeDelta timeout);

  
  
  
  bool WaitForFindWindowVisibilityChange(BrowserProxy* browser,
                                         bool wait_for_open);

  
  void TerminateBrowser();

  
  
  
  
  
  void NavigateToURLAsync(const GURL& url);

 private:
  base::MessageLoop message_loop_;  
};

#ifdef UNIT_TEST
std::ostream& operator<<(std::ostream& out, const std::wstring& wstr);

template<typename T>
std::ostream& operator<<(std::ostream& out, const ::scoped_ptr<T>& ptr) {
  return out << ptr.get();
}
#endif  

#endif  
