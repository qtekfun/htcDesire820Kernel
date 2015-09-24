// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_FULLSCREEN_MODE_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_FULLSCREEN_MODE_CONTROLLER_H_

#include <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>

#import "base/mac/scoped_nsobject.h"
#import "ui/base/cocoa/tracking_area.h"

@class BrowserWindowController;

@interface FullscreenModeController : NSObject<NSAnimationDelegate> {
 @private
  enum FullscreenToolbarState {
    kFullscreenToolbarOnly,
    kFullscreenToolbarAndTabstrip,
  };

  
  BrowserWindowController* controller_;

  
  
  ui::ScopedCrTrackingArea trackingArea_;

  
  
  base::scoped_nsobject<NSAnimation> animation_;

  
  
  FullscreenToolbarState destinationState_;
  FullscreenToolbarState currentState_;

  
  EventHandlerRef menuBarTrackingHandler_;

  
  
  CGFloat menuBarRevealFraction_;
}

- (id)initWithBrowserWindowController:(BrowserWindowController*)bwc;

- (CGFloat)menuBarHeight;

@end

#endif  
