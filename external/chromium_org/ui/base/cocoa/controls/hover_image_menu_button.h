// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_COCOA_CONTROLS_HOVER_IMAGE_MENU_BUTTON_H_
#define UI_BASE_COCOA_CONTROLS_HOVER_IMAGE_MENU_BUTTON_H_

#import <Cocoa/Cocoa.h>

#import "ui/base/cocoa/tracking_area.h"
#include "ui/base/ui_export.h"

@class HoverImageMenuButtonCell;

UI_EXPORT
@interface HoverImageMenuButton : NSPopUpButton {
 @private
  ui::ScopedCrTrackingArea trackingArea_;
}

- (HoverImageMenuButtonCell*)hoverImageMenuButtonCell;

@end

#endif  
