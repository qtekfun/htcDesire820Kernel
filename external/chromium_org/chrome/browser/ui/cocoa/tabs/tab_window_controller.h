// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABS_TAB_WINDOW_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TABS_TAB_WINDOW_CONTROLLER_H_


#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"

@class FastResizeView;
@class FocusTracker;
@class TabStripView;
@class TabView;

@interface TabWindowController : NSWindowController<NSWindowDelegate> {
 @private
  base::scoped_nsobject<FastResizeView> tabContentArea_;
  base::scoped_nsobject<TabStripView> tabStripView_;

  
  NSWindow* overlayWindow_;

  
  
  NSView* originalContentView_;  

  base::scoped_nsobject<FocusTracker> focusBeforeOverlay_;
  BOOL closeDeferred_;  
}
@property(readonly, nonatomic) TabStripView* tabStripView;
@property(readonly, nonatomic) FastResizeView* tabContentArea;

- (id)initTabWindowControllerWithTabStrip:(BOOL)hasTabStrip;

- (void)showOverlay;
- (void)removeOverlay;
- (NSWindow*)overlayWindow;

- (BOOL)shouldConstrainFrameRect;


- (void)layoutTabs;

- (TabWindowController*)detachTabToNewWindow:(TabView*)tabView;

- (void)insertPlaceholderForTab:(TabView*)tab frame:(NSRect)frame;

- (void)removePlaceholder;

- (BOOL)isDragSessionActive;

- (BOOL)tabDraggingAllowed;
- (BOOL)tabTearingAllowed;
- (BOOL)windowMovementAllowed;

- (void)showNewTabButton:(BOOL)show;

- (BOOL)isTabFullyVisible:(TabView*)tab;

- (BOOL)canReceiveFrom:(TabWindowController*)source;

- (void)moveTabView:(NSView*)view
     fromController:(TabWindowController*)controller;

- (NSInteger)numberOfTabs;

- (BOOL)hasLiveTabs;

- (NSView*)activeTabView;

- (NSString*)activeTabTitle;

- (BOOL)hasTabStrip;

- (BOOL)isTabDraggable:(NSView*)tabView;

- (void)deferPerformClose;

@end

@interface TabWindowController(ProtectedMethods)
- (void)detachTabView:(NSView*)view;

- (void)layoutSubviews;
@end

#endif  
