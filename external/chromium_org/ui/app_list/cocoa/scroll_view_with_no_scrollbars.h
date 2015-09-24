// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_COCOA_SCROLL_VIEW_WITH_NO_SCROLLBARS_H_
#define UI_APP_LIST_COCOA_SCROLL_VIEW_WITH_NO_SCROLLBARS_H_

#include <Cocoa/Cocoa.h>

@protocol GestureScrollDelegate

- (void)userScrolling:(BOOL)isScrolling;

@end

@interface ScrollViewWithNoScrollbars : NSScrollView {
 @private
  id<GestureScrollDelegate> delegate_;
}

@property(assign, nonatomic) id<GestureScrollDelegate> delegate;

@end

#endif  
