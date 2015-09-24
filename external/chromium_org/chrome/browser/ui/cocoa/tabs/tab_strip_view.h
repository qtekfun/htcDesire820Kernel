// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABS_TAB_STRIP_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_TABS_TAB_STRIP_VIEW_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#import "chrome/browser/ui/cocoa/background_gradient_view.h"
#import "chrome/browser/ui/cocoa/url_drop_target.h"

@class NewTabButton;
@class TabStripController;


@interface TabStripView : BackgroundGradientView<URLDropTarget> {
 @private
  TabStripController* controller_;  

  NSTimeInterval lastMouseUp_;

  
  base::scoped_nsobject<URLDropTargetHandler> dropHandler_;

  base::scoped_nsobject<NewTabButton> newTabButton_;

  
  
  BOOL dropArrowShown_;
  NSPoint dropArrowPosition_;
}

@property(assign, nonatomic) BOOL dropArrowShown;
@property(assign, nonatomic) NSPoint dropArrowPosition;

- (NewTabButton*)getNewTabButton;

@end

@interface TabStripView (TabStripControllerInterface)
- (void)setController:(TabStripController*)controller;
@end

@interface TabStripView (Protected)
- (void)drawBottomBorder:(NSRect)bounds;
- (BOOL)doubleClickMinimizesWindow;
@end

@interface TabStripView (TestingAPI)
- (void)setNewTabButton:(NewTabButton*)button;
@end

#endif  
