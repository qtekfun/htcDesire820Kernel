// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_CONSTRAINED_WINDOW_BUTTON_
#define CHROME_BROWSER_UI_COCOA_CONSTRAINED_WINDOW_CONSTRAINED_WINDOW_BUTTON_

#import <Cocoa/Cocoa.h>

#import "ui/base/cocoa/tracking_area.h"

@interface ConstrainedWindowButton : NSButton {
 @private
  ui::ScopedCrTrackingArea trackingArea_;
}

@end

@interface ConstrainedWindowButtonCell : NSButtonCell {
 @private
  BOOL isMouseInside_;
}

@property(nonatomic, assign) BOOL isMouseInside;

@end

#endif  
