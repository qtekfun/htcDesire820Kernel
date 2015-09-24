// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_COCOA_APP_LIST_PAGER_VIEW_H_
#define UI_APP_LIST_COCOA_APP_LIST_PAGER_VIEW_H_

#import <Cocoa/Cocoa.h>

#import "ui/base/cocoa/tracking_area.h"

@class AppListPagerView;

@protocol AppListPagerDelegate<NSObject>

- (CGFloat)visiblePortionOfPage:(int)page;

- (void)onPagerClicked:(AppListPagerView*)sender;

@end

@interface AppListPagerView : NSSegmentedControl {
 @private
  
  ui::ScopedCrTrackingArea trackingArea_;

  
  NSInteger hoveredSegment_;
}

- (NSInteger)findAndHighlightSegmentAtLocation:(NSPoint)locationInWindow;

@end

#endif  
