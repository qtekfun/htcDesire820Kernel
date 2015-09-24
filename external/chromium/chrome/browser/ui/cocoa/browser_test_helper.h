// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BROWSER_TEST_HELPER_H_
#define CHROME_BROWSER_UI_COCOA_BROWSER_TEST_HELPER_H_
#pragma once

#include "chrome/browser/ui/browser.h"
#include "chrome/test/testing_profile.h"
#include "content/browser/browser_thread.h"

class BrowserTestHelper {
 public:
  BrowserTestHelper();
  virtual ~BrowserTestHelper();

  virtual TestingProfile* profile() const;
  Browser* browser() const { return browser_.get(); }

  
  
  BrowserWindow* CreateBrowserWindow();

  
  
  void CloseBrowserWindow();

 private:
  scoped_ptr<TestingProfile> profile_;
  scoped_ptr<Browser> browser_;
  MessageLoopForUI message_loop_;
  BrowserThread ui_thread_;
  scoped_ptr<BrowserThread> file_thread_;
  scoped_ptr<BrowserThread> io_thread_;
};

#endif  
