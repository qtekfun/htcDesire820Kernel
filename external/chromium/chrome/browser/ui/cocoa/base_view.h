// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_BASE_VIEW_H_
#define CHROME_BROWSER_UI_COCOA_BASE_VIEW_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "ui/gfx/rect.h"


@interface BaseView : NSView {
 @private
  NSTrackingArea *trackingArea_;
  BOOL dragging_;
  scoped_nsobject<NSEvent> pendingExitEvent_;
}

- (id)initWithFrame:(NSRect)frame;

- (void)mouseEvent:(NSEvent *)theEvent;
- (void)keyEvent:(NSEvent *)theEvent;

- (gfx::Rect)flipNSRectToRect:(NSRect)rect;
- (NSRect)flipRectToNSRect:(gfx::Rect)rect;

@end

extern NSString* kViewDidBecomeFirstResponder;
extern NSString* kSelectionDirection;

#endif  
