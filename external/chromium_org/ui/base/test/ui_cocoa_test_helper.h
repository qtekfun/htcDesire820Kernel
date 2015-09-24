// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_TEST_UI_COCOA_TEST_HELPER_H_
#define UI_BASE_TEST_UI_COCOA_TEST_HELPER_H_

#include <set>

#import <Cocoa/Cocoa.h>

#include "base/compiler_specific.h"
#import "base/mac/scoped_nsautorelease_pool.h"
#include "testing/platform_test.h"

@interface CocoaTestHelperWindow : NSWindow {
 @private
  BOOL pretendIsKeyWindow_;
}

- (id)initWithContentRect:(NSRect)contentRect;

- (id)init;

- (void)makePretendKeyWindowAndSetFirstResponder:(NSResponder*)responder;

- (void)clearPretendKeyWindowAndFirstResponder;

- (void)setPretendIsKeyWindow:(BOOL)isKeyWindow;

- (BOOL)isKeyWindow;

@end

namespace ui {

class CocoaTest : public PlatformTest {
 public:
  CocoaTest();
  virtual ~CocoaTest();

  
  
  
  
  
  
  virtual void TearDown() OVERRIDE;

  
  
  
  
  CocoaTestHelperWindow* test_window();

 protected:
  
  
  void Init();

 private:
  
  
  
  
  static std::set<NSWindow*> ApplicationWindows();

  
  
  std::set<NSWindow*> WindowsLeft();

  bool called_tear_down_;
  base::mac::ScopedNSAutoreleasePool pool_;

  
  std::set<NSWindow*> initial_windows_;

  
  
  
  
  
  
  
  CocoaTestHelperWindow* test_window_;
};

}  

#define TEST_VIEW(test_fixture, test_view)                      \
  TEST_F(test_fixture, test_fixture##_TestViewMacroAddRemove) { \
    base::scoped_nsobject<NSView> view([test_view retain]);     \
    EXPECT_EQ([test_window() contentView], [view superview]);   \
    [view removeFromSuperview];                                 \
    EXPECT_FALSE([view superview]);                             \
  }                                                             \
  TEST_F(test_fixture, test_fixture##_TestViewMacroDisplay) {   \
    [test_view display];                                        \
  }

#if CGFLOAT_IS_DOUBLE
#define CGFLOAT_EPSILON DBL_EPSILON
#else
#define CGFLOAT_EPSILON FLT_EPSILON
#endif

#define CGFLOAT_EQ(expected, actual) \
    (actual >= (expected - CGFLOAT_EPSILON) && \
     actual <= (expected + CGFLOAT_EPSILON))

#define EXPECT_CGFLOAT_EQ(expected, actual) \
    EXPECT_TRUE(CGFLOAT_EQ(expected, actual)) << \
                expected << " != " << actual

#define EXPECT_NSRECT_EQ(expected, actual) \
    EXPECT_TRUE(CGFLOAT_EQ(expected.origin.x, actual.origin.x) && \
                CGFLOAT_EQ(expected.origin.y, actual.origin.y) && \
                CGFLOAT_EQ(expected.size.width, actual.size.width) && \
                CGFLOAT_EQ(expected.size.height, actual.size.height)) << \
                "Rects do not match: " << \
                [NSStringFromRect(expected) UTF8String] << \
                " != " << [NSStringFromRect(actual) UTF8String]

#endif  
