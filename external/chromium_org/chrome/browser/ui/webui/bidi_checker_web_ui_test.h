// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_BIDI_CHECKER_WEB_UI_TEST_H_
#define CHROME_BROWSER_UI_WEBUI_BIDI_CHECKER_WEB_UI_TEST_H_

#include "base/command_line.h"
#include "chrome/test/base/web_ui_browsertest.h"

namespace base {
class WaitableEvent;
}

class WebUIBidiCheckerBrowserTest : public WebUIBrowserTest {
 public:
  virtual ~WebUIBidiCheckerBrowserTest();

  
  virtual void SetUp() OVERRIDE;
  virtual void TearDown() OVERRIDE;

 protected:
  WebUIBidiCheckerBrowserTest();

  
  
  void RunBidiCheckerOnPage(const std::string& page_url, bool is_rtl);

  
  virtual void SetUpInProcessBrowserTestFixture() OVERRIDE;

 private:
  
  CommandLine::StringVector argv_;
};

class WebUIBidiCheckerBrowserTestLTR : public WebUIBidiCheckerBrowserTest {
 public:
  void RunBidiCheckerOnPage(const std::string& page_url);
};

class WebUIBidiCheckerBrowserTestRTL : public WebUIBidiCheckerBrowserTest {
 public:
  void RunBidiCheckerOnPage(const std::string& page_url);

 protected:
  virtual void SetUpOnMainThread() OVERRIDE;
  virtual void CleanUpOnMainThread() OVERRIDE;

  
  std::string app_locale_;
};

#endif  
