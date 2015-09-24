// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_COCOA_HOVER_BUTTON_
#define UI_BASE_COCOA_HOVER_BUTTON_

#import <Cocoa/Cocoa.h>

#import "ui/base/cocoa/tracking_area.h"
#import "ui/base/ui_export.h"

UI_EXPORT
@interface HoverButton : NSButton {
 @protected
  
  
  enum HoverState {
    kHoverStateNone = 0,
    kHoverStateMouseOver = 1,
    kHoverStateMouseDown = 2
  };

  HoverState hoverState_;

 @private
  
  ui::ScopedCrTrackingArea trackingArea_;
}

@property(nonatomic) HoverState hoverState;

- (void)setTrackingEnabled:(BOOL)enabled;

- (void)checkImageState;

@end

#endif  
