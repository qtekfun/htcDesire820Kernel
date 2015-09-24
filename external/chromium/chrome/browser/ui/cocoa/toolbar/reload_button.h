// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TOOLBAR_RELOAD_BUTTON_H_
#define CHROME_BROWSER_UI_COCOA_TOOLBAR_RELOAD_BUTTON_H_
#pragma once

#import <Cocoa/Cocoa.h>

#import "base/memory/scoped_nsobject.h"
#import "chrome/browser/ui/cocoa/toolbar/toolbar_button.h"


@interface ReloadButton : ToolbarButton {
 @private
  
  
  BOOL isMouseInside_;
  scoped_nsobject<NSTrackingArea> trackingArea_;

  
  scoped_nsobject<NSTimer> pendingReloadTimer_;
}

- (BOOL)isMouseInside;

- (void)updateTag:(NSInteger)anInt;

- (void)setIsLoading:(BOOL)isLoading force:(BOOL)force;

@end

@interface ReloadButton (PrivateTestingMethods)
+ (void)setPendingReloadTimeout:(NSTimeInterval)seconds;
- (NSTrackingArea*)trackingArea;
@end

#endif  
