// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_PROFILE_TEST_H_
#define CHROME_BROWSER_UI_COCOA_PROFILE_TEST_H_

#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/cocoa_test_helper.h"
#include "chrome/test/base/testing_profile_manager.h"

namespace content {
class TestBrowserThreadBundle;
}

class Browser;
class TestingProfile;

class CocoaProfileTest : public CocoaTest {
 public:
  CocoaProfileTest();
  virtual ~CocoaProfileTest();

  
  
  
  
  virtual void SetUp() OVERRIDE;

  virtual void TearDown() OVERRIDE;

  TestingProfileManager* testing_profile_manager() {
    return &profile_manager_;
  }
  TestingProfile* profile() { return profile_; }
  Browser* browser() { return browser_.get(); }

  
  
  void CloseBrowserWindow();

 protected:
  
  
  virtual Browser* CreateBrowser();

 private:
  TestingProfileManager profile_manager_;
  TestingProfile* profile_;  
  scoped_ptr<Browser> browser_;

  scoped_ptr<content::TestBrowserThreadBundle> thread_bundle_;
};

#endif  
