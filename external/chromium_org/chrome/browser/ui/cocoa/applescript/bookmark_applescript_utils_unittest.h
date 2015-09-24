// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_APPLESCRIPT_BOOKMARK_APPLESCRIPT_UTILS_UNITTEST_H_
#define CHROME_BROWSER_UI_COCOA_APPLESCRIPT_BOOKMARK_APPLESCRIPT_UTILS_UNITTEST_H_

#import <Cocoa/Cocoa.h>
#import <objc/objc-runtime.h>

#include "base/mac/scoped_nsobject.h"
#import "chrome/browser/app_controller_mac.h"
#import "chrome/browser/ui/cocoa/applescript/bookmark_folder_applescript.h"
#include "chrome/browser/ui/cocoa/cocoa_profile_test.h"
#include "testing/platform_test.h"

class BookmarkModel;

@interface FakeAppDelegate : AppController {
 @public
  CocoaProfileTest* test_;  
}
@property(nonatomic) CocoaProfileTest* test;
- (Profile*)lastProfile;
@end


@interface FakeScriptCommand : NSScriptCommand {
  Method originalMethod_;
  Method alternateMethod_;
}
@end


class BookmarkAppleScriptTest : public CocoaProfileTest {
 public:
  BookmarkAppleScriptTest();
  virtual ~BookmarkAppleScriptTest();
  virtual void SetUp() OVERRIDE;
 private:
  base::scoped_nsobject<FakeAppDelegate> appDelegate_;

 protected:
  base::scoped_nsobject<BookmarkFolderAppleScript> bookmarkBar_;
};

#endif  
