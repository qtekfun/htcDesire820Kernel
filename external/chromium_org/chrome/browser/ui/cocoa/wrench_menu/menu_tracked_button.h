// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_WRENCH_MENU_MENU_TRACKED_BUTTON_H_
#define CHROME_BROWSER_UI_COCOA_WRENCH_MENU_MENU_TRACKED_BUTTON_H_

#import <Cocoa/Cocoa.h>

@interface MenuTrackedButton : NSButton {
 @private
  
  
  BOOL didEnter_;

  
  
  BOOL tracking_;

  
  
  NSTrackingRectTag trackingTag_;
}

@property(nonatomic, readonly, getter=isTracking) BOOL tracking;

@end

#endif  
