// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABS_TAB_STRIP_DRAG_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TABS_TAB_STRIP_DRAG_CONTROLLER_H_

#import <Cocoa/Cocoa.h>
#include <map>

@class TabController;
@class TabStripController;
@class TabWindowController;


@protocol TabDraggingEventTarget

- (BOOL)tabCanBeDragged:(TabController*)tab;

- (void)maybeStartDrag:(NSEvent*)event forTab:(TabController*)tab;

@end


@interface TabStripDragController : NSObject<TabDraggingEventTarget> {
 @private
  TabStripController* tabStrip_;  

  
  BOOL moveWindowOnDrag_;  
  BOOL tabWasDragged_;  
  BOOL draggingWithinTabStrip_;  
  BOOL chromeIsVisible_;

  NSTimeInterval tearTime_;  
  NSPoint tearOrigin_;  
  NSPoint dragOrigin_;  

  TabWindowController* sourceController_;  
  NSWindow* sourceWindow_;  
  NSRect sourceWindowFrame_;
  NSRect sourceTabFrame_;

  TabController* draggedTab_;  

  TabWindowController* draggedController_;  
  NSWindow* dragWindow_;  
  NSWindow* dragOverlay_;  

  TabWindowController* targetController_;  
}

- (id)initWithTabStripController:(TabStripController*)controller;


@end

#endif  
