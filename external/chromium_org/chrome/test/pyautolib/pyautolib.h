// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_PYAUTOLIB_PYAUTOLIB_H_
#define CHROME_TEST_PYAUTOLIB_PYAUTOLIB_H_

#include "base/compiler_specific.h"
#include "base/message_loop/message_loop.h"
#include "base/test/test_timeouts.h"
#include "base/time/time.h"
#include "chrome/test/ui/ui_test.h"
#include "chrome/test/ui/ui_test_suite.h"

#if defined(OS_MACOSX)
#include "base/mac/scoped_nsautorelease_pool.h"
#endif

class AutomationProxy;


class PyUITestSuiteBase : public UITestSuite {
 public:
  PyUITestSuiteBase(int argc, char** argv);
  virtual ~PyUITestSuiteBase();

  void InitializeWithPath(const base::FilePath& browser_dir);

  void SetCrSourceRoot(const base::FilePath& path);

 private:
#if defined(OS_MACOSX)
  base::mac::ScopedNSAutoreleasePool pool_;
#endif
};

class PyUITestBase : public UITestBase {
 public:
  

  
  PyUITestBase(bool clear_profile, std::wstring homepage);
  virtual ~PyUITestBase();

  
  
  void Initialize(const base::FilePath& browser_dir);

  void UseNamedChannelID(const std::string& named_channel_id) {
    named_channel_id_ = named_channel_id;
    launcher_.reset(CreateProxyLauncher());
  }

  virtual ProxyLauncher* CreateProxyLauncher() OVERRIDE;

  
  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

  

  
  
  
  
  
  std::string _SendJSONRequest(int window_index,
                               const std::string& request,
                               int timeout);

  
  bool SetCookie(const GURL& cookie_url, const std::string& value,
                 int window_index = 0, int tab_index = 0);
  
  std::string GetCookie(const GURL& cookie_url, int window_index = 0,
                        int tab_index = 0);

 protected:
  
  virtual AutomationProxy* automation() const OVERRIDE;

  virtual void SetLaunchSwitches() OVERRIDE;

 private:
  
  void ErrorResponse(const std::string& error_string,
                     const std::string& request,
                     bool is_timeout,
                     std::string* response);
  void RequestFailureResponse(
      const std::string& request,
      const base::TimeDelta& duration,
      const base::TimeDelta& timeout,
      std::string* response);

  
  
  
  
  
  static base::MessageLoop* GetSharedMessageLoop(
      base::MessageLoop::Type msg_loop_type);
  static base::MessageLoop* message_loop_;

  
  std::string named_channel_id_;
};

#endif  
