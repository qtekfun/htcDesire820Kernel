// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_PRESENTATION_MODE_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_PRESENTATION_MODE_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/mac_util.h"
#include "chrome/browser/ui/cocoa/location_bar/location_bar_view_mac.h"

@class BrowserWindowController;
@class DropdownAnimation;

@interface PresentationModeController : NSObject<NSAnimationDelegate> {
 @private
  
  BrowserWindowController* browserController_;  

  
  
  NSView* contentView_;  

  
  BOOL enteringPresentationMode_;

  
  BOOL inPresentationMode_;

  
  
  
  
  
  base::scoped_nsobject<NSTrackingArea> trackingArea_;

  
  
  base::scoped_nsobject<DropdownAnimation> currentAnimation_;

  
  
  base::scoped_nsobject<NSTimer> showTimer_;
  base::scoped_nsobject<NSTimer> hideTimer_;

  
  
  
  NSRect trackingAreaBounds_;

  
  
  
  
  
  
  
  
  
  base::mac::FullScreenMode systemFullscreenMode_;
}

@property(readonly, nonatomic) BOOL inPresentationMode;

- (id)initWithBrowserController:(BrowserWindowController*)controller;

- (void)enterPresentationModeForContentView:(NSView*)contentView
                               showDropdown:(BOOL)showDropdown;
- (void)exitPresentationMode;

- (CGFloat)floatingBarVerticalOffset;

- (void)overlayFrameChanged:(NSRect)frame;

- (void)ensureOverlayShownWithAnimation:(BOOL)animate delay:(BOOL)delay;
- (void)ensureOverlayHiddenWithAnimation:(BOOL)animate delay:(BOOL)delay;

- (void)cancelAnimationAndTimers;

- (CGFloat)floatingBarShownFraction;

- (void)changeFloatingBarShownFraction:(CGFloat)fraction;

@end

extern NSString* const kWillEnterFullscreenNotification;
extern NSString* const kWillLeaveFullscreenNotification;

#endif  
