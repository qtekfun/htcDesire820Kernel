// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"

@class InfoBubbleView;
class TabStripModelObserverBridge;

@interface BaseBubbleController : NSWindowController<NSWindowDelegate> {
 @private
  NSWindow* parentWindow_;  
  NSPoint anchor_;
  IBOutlet InfoBubbleView* bubble_;  
  
  scoped_ptr<TabStripModelObserverBridge> tabStripObserverBridge_;

  
  
  
  
  id eventTap_;
  
  id resignationObserver_;
  
  
  BOOL shouldOpenAsKeyWindow_;
  
  BOOL shouldCloseOnResignKey_;
}

@property(nonatomic, readonly) NSWindow* parentWindow;
@property(nonatomic, assign) NSPoint anchorPoint;
@property(nonatomic, readonly) InfoBubbleView* bubble;
@property(nonatomic, assign) BOOL shouldOpenAsKeyWindow;
@property(nonatomic, assign) BOOL shouldCloseOnResignKey;

- (id)initWithWindowNibPath:(NSString*)nibPath
               parentWindow:(NSWindow*)parentWindow
                 anchoredAt:(NSPoint)anchoredAt;


- (id)initWithWindowNibPath:(NSString*)nibPath
             relativeToView:(NSView*)view
                     offset:(NSPoint)offset;


- (id)initWithWindow:(NSWindow*)theWindow
        parentWindow:(NSWindow*)parentWindow
          anchoredAt:(NSPoint)anchoredAt;

- (NSBox*)separatorWithFrame:(NSRect)frame;

@end

@interface BaseBubbleController (Protected)
- (void)registerKeyStateEventTap;
@end
