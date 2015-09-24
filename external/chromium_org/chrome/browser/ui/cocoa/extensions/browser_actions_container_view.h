// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_BROWSER_ACTIONS_CONTAINER_VIEW_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_BROWSER_ACTIONS_CONTAINER_VIEW_

#import <Cocoa/Cocoa.h>

extern NSString* const kBrowserActionGrippyDragStartedNotification;

extern NSString* const kBrowserActionGrippyDraggingNotification;

extern NSString* const kBrowserActionGrippyDragFinishedNotification;

@interface BrowserActionsContainerView : NSView {
 @private
  
  NSRect grippyRect_;

  
  
  NSRect animationEndFrame_;

  
  
  NSPoint initialDragPoint_;

  
  CGFloat lastXPos_;

  
  CGFloat maxWidth_;

  
  BOOL userIsResizing_;

  
  
  
  BOOL resizable_;

  
  
  
  BOOL canDragLeft_;

  
  
  BOOL canDragRight_;

  
  
  
  
  BOOL grippyPinned_;
}

- (void)resizeToWidth:(CGFloat)width animate:(BOOL)animate;

- (CGFloat)resizeDeltaX;

@property(nonatomic, readonly) NSRect animationEndFrame;
@property(nonatomic) BOOL canDragLeft;
@property(nonatomic) BOOL canDragRight;
@property(nonatomic) BOOL grippyPinned;
@property(nonatomic,getter=isResizable) BOOL resizable;
@property(nonatomic) CGFloat maxWidth;
@property(readonly, nonatomic) BOOL userIsResizing;

@end

#endif  
