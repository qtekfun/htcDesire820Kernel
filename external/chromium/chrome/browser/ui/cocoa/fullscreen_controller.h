// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_FULLSCREEN_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_FULLSCREEN_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "base/mac/mac_util.h"
#include "chrome/browser/ui/cocoa/location_bar/location_bar_view_mac.h"

@class BrowserWindowController;
@class DropdownAnimation;

@interface FullscreenController : NSObject<NSAnimationDelegate> {
 @private
  
  BrowserWindowController* browserController_;  

  
  
  NSView* contentView_;  

  
  BOOL isFullscreen_;

  
  
  
  
  
  scoped_nsobject<NSTrackingArea> trackingArea_;

  
  
  scoped_nsobject<DropdownAnimation> currentAnimation_;

  
  
  scoped_nsobject<NSTimer> showTimer_;
  scoped_nsobject<NSTimer> hideTimer_;

  
  
  
  NSRect trackingAreaBounds_;

  
  
  
  
  
  
  
  
  
  base::mac::FullScreenMode currentFullscreenMode_;
}

@property(readonly, nonatomic) BOOL isFullscreen;

- (id)initWithBrowserController:(BrowserWindowController*)controller;

- (void)enterFullscreenForContentView:(NSView*)contentView
                         showDropdown:(BOOL)showDropdown;
- (void)exitFullscreen;

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
