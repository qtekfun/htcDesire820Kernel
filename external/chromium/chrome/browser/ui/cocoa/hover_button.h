// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"

@interface HoverButton : NSButton {
 @protected
  
  
  enum HoverState {
    kHoverStateNone = 0,
    kHoverStateMouseOver = 1,
    kHoverStateMouseDown = 2
  };

  HoverState hoverState_;

 @private
  
  scoped_nsobject<NSTrackingArea> trackingArea_;
}

- (void)setTrackingEnabled:(BOOL)enabled;

- (void)checkImageState;
@end
