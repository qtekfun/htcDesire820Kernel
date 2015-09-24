// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_COCOA_TEST_HELPER_H_
#define CHROME_BROWSER_UI_COCOA_COCOA_TEST_HELPER_H_

#import <Cocoa/Cocoa.h>

#import "ui/base/test/ui_cocoa_test_helper.h"

class CocoaTest : public ui::CocoaTest {
 public:
  
  
  
  
  static void BootstrapCocoa();

  CocoaTest();
};

#endif  
