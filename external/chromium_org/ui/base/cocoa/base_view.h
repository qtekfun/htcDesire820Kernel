// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_COCOA_BASE_VIEW_H_
#define UI_BASE_COCOA_BASE_VIEW_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#import "ui/base/cocoa/tracking_area.h"
#include "ui/base/ui_export.h"
#include "ui/gfx/rect.h"

UI_EXPORT
@interface BaseView : NSView {
 @public
  enum EventHandled {
    kEventNotHandled,
    kEventHandled
  };

 @private
  ui::ScopedCrTrackingArea trackingArea_;
  BOOL dragging_;
  base::scoped_nsobject<NSEvent> pendingExitEvent_;
}

- (void)mouseEvent:(NSEvent *)theEvent;

- (EventHandled)keyEvent:(NSEvent *)theEvent;

- (gfx::Rect)flipNSRectToRect:(NSRect)rect;
- (NSRect)flipRectToNSRect:(gfx::Rect)rect;

@end

UI_EXPORT extern NSString* kViewDidBecomeFirstResponder;
UI_EXPORT extern NSString* kSelectionDirection;

#endif  
