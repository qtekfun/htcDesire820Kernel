// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABS_TAB_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_TABS_TAB_VIEW_H_

#include <ApplicationServices/ApplicationServices.h>
#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_cftyperef.h"
#include "base/mac/scoped_nsobject.h"
#import "chrome/browser/ui/cocoa/hover_close_button.h"

namespace tabs {


enum AlertState {
  kAlertNone = 0,  
  kAlertRising,
  kAlertHolding,
  kAlertFalling
};

const CGFloat kImageNoFocusAlpha = 0.65;

}  

@class TabController, TabWindowController;


@interface TabView : NSView {
 @private
  TabController* controller_;
  
  
  HoverCloseButton* closeButton_;  

  BOOL closing_;

  BOOL isMouseInside_;  
  tabs::AlertState alertState_;

  CGFloat hoverAlpha_;  
  NSTimeInterval hoverHoldEndTime_;  

  CGFloat alertAlpha_;  
  NSTimeInterval alertHoldEndTime_;  

  NSTimeInterval lastGlowUpdate_;  

  NSPoint hoverPoint_;  

  
  NSPoint mouseDownPoint_;

  NSCellStateValue state_;

  
  base::scoped_nsobject<NSString> toolTipText_;

  
  
  base::ScopedCFTypeRef<CGImageRef> maskCache_;
  CGFloat maskCacheWidth_;
  CGFloat maskCacheScale_;
}

@property(assign, nonatomic) NSCellStateValue state;
@property(assign, nonatomic) CGFloat hoverAlpha;
@property(assign, nonatomic) CGFloat alertAlpha;

@property(assign, nonatomic, getter=isClosing) BOOL closing;

- (id)initWithFrame:(NSRect)frame
         controller:(TabController*)controller
        closeButton:(HoverCloseButton*)closeButton;

+ (CGFloat)insetMultiplier;

- (void)setTrackingEnabled:(BOOL)enabled;

- (void)startAlert;

- (void)cancelAlert;

- (NSString*)toolTipText;

@end

@interface TabView (TabControllerInterface)
- (void)setController:(TabController*)controller;
@end

#endif  
