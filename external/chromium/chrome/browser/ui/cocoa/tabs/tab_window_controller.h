// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABS_TAB_WINDOW_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TABS_TAB_WINDOW_CONTROLLER_H_
#pragma once


#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_nsobject.h"

@class FastResizeView;
@class FocusTracker;
@class TabStripView;
@class TabView;

@interface TabWindowController : NSWindowController<NSWindowDelegate> {
 @private
  IBOutlet FastResizeView* tabContentArea_;
  
  
  IBOutlet TabStripView* topTabStripView_;
  IBOutlet TabStripView* sideTabStripView_;
  NSWindow* overlayWindow_;  
  NSView* cachedContentView_;  
                               
                               
  scoped_nsobject<FocusTracker> focusBeforeOverlay_;
  scoped_nsobject<NSMutableSet> lockedTabs_;
  BOOL closeDeferred_;  
  
  
  
  CGFloat contentAreaHeightDelta_;
}
@property(readonly, nonatomic) TabStripView* tabStripView;
@property(readonly, nonatomic) FastResizeView* tabContentArea;

- (void)showOverlay;
- (void)removeOverlay;
- (NSWindow*)overlayWindow;

- (BOOL)shouldConstrainFrameRect;


- (void)layoutTabs;

- (TabWindowController*)detachTabToNewWindow:(TabView*)tabView;

- (void)insertPlaceholderForTab:(TabView*)tab
                          frame:(NSRect)frame
                  yStretchiness:(CGFloat)yStretchiness;

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

- (NSView *)selectedTabView;

- (NSString*)selectedTabTitle;

- (BOOL)hasTabStrip;

- (BOOL)useVerticalTabs;

- (BOOL)isTabDraggable:(NSView*)tabView;
- (void)setTab:(NSView*)tabView isDraggable:(BOOL)draggable;

- (void)deferPerformClose;

@end

@interface TabWindowController(ProtectedMethods)
- (void)detachTabView:(NSView*)view;

- (void)toggleTabStripDisplayMode;

- (void)layoutSubviews;
@end

#endif  
