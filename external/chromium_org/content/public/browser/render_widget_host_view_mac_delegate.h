// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_RENDER_WIDGET_HOST_VIEW_MAC_DELEGATE_H_
#define CONTENT_PUBLIC_BROWSER_RENDER_WIDGET_HOST_VIEW_MAC_DELEGATE_H_

#import <Cocoa/Cocoa.h>


@class NSEvent;
@protocol RenderWidgetHostViewMacDelegate
@optional
- (void)viewGone:(NSView*)view;

- (BOOL)handleEvent:(NSEvent*)event;

- (void)gotUnhandledWheelEvent;

- (void)scrollOffsetPinnedToLeft:(BOOL)left toRight:(BOOL)right;

- (void)setHasHorizontalScrollbar:(BOOL)has_horizontal_scrollbar;

- (BOOL)validateUserInterfaceItem:(id<NSValidatedUserInterfaceItem>)item
                      isValidItem:(BOOL*)valid;

@required
- (void)beginGestureWithEvent:(NSEvent*)event;
- (void)endGestureWithEvent:(NSEvent*)event;

- (void)touchesMovedWithEvent:(NSEvent*)event;
- (void)touchesBeganWithEvent:(NSEvent*)event;
- (void)touchesCancelledWithEvent:(NSEvent*)event;
- (void)touchesEndedWithEvent:(NSEvent*)event;

@end

#endif  
