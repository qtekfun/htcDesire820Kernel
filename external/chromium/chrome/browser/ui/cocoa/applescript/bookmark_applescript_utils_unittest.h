// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_APPLESCRIPT_BOOKMARK_APPLESCRIPT_UTILS_UNITTEST_H_
#define CHROME_BROWSER_UI_COCOA_APPLESCRIPT_BOOKMARK_APPLESCRIPT_UTILS_UNITTEST_H_

#import <objc/objc-runtime.h>
#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#import "chrome/browser/app_controller_mac.h"
#import "chrome/browser/ui/cocoa/applescript/bookmark_folder_applescript.h"
#include "chrome/browser/ui/cocoa/browser_test_helper.h"
#include "chrome/browser/ui/cocoa/cocoa_test_helper.h"
#include "chrome/test/model_test_utils.h"
#include "testing/platform_test.h"

class BookmarkModel;

@interface FakeAppDelegate : AppController {
 @public
  BrowserTestHelper* helper_;  
}
@property(nonatomic) BrowserTestHelper* helper;
- (Profile*)defaultProfile;
@end


@interface FakeScriptCommand : NSScriptCommand {
  Method originalMethod_;
  Method alternateMethod_;
}
@end


class BookmarkAppleScriptTest : public CocoaTest {
 public:
  BookmarkAppleScriptTest();
  virtual ~BookmarkAppleScriptTest();
 private:
  BrowserTestHelper helper_;
  scoped_nsobject<FakeAppDelegate> appDelegate_;
 protected:
  scoped_nsobject<BookmarkFolderAppleScript> bookmarkBar_;
  BookmarkModel& model();
};

#endif
