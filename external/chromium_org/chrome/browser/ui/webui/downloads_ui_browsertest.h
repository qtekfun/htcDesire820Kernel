// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_DOWNLOADS_UI_BROWSERTEST_H_
#define CHROME_BROWSER_UI_WEBUI_DOWNLOADS_UI_BROWSERTEST_H_

#include "chrome/test/base/web_ui_browsertest.h"

class DownloadsUIBrowserTest : public WebUIBrowserTest {
 public:
  DownloadsUIBrowserTest();
  virtual ~DownloadsUIBrowserTest();

 protected:
  
  void SetDeleteAllowed(bool allowed);

 private:
  DISALLOW_COPY_AND_ASSIGN(DownloadsUIBrowserTest);
};

class DownloadsWebUIForSupervisedUsersTest : public DownloadsUIBrowserTest {
 public:
  
  virtual void SetUpCommandLine(CommandLine* command_line) OVERRIDE;
};

#endif  
