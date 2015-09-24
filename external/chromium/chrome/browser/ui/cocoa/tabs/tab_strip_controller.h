// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABS_TAB_STRIP_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TABS_TAB_STRIP_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/tab_contents/tab_contents_controller.h"
#import "chrome/browser/ui/cocoa/tabs/tab_controller_target.h"
#import "chrome/browser/ui/cocoa/url_drop_target.h"
#import "third_party/GTM/AppKit/GTMWindowSheetController.h"

@class CrTrackingArea;
@class NewTabButton;
@class ProfileMenuButton;
@class TabContentsController;
@class TabView;
@class TabStripView;

class Browser;
class ConstrainedWindowMac;
class TabStripModelObserverBridge;
class TabStripModel;
class TabContents;
class ToolbarModel;

namespace TabStripControllerInternal {
class NotificationBridge;
} 

@protocol TabStripControllerDelegate

- (void)onSelectTabWithContents:(TabContents*)contents;

- (void)onReplaceTabWithContents:(TabContents*)contents;

- (void)onSelectedTabChange:(TabStripModelObserver::TabChangeType)change;

- (void)onTabDetachedWithContents:(TabContents*)contents;

@end

@interface TabStripController :
  NSObject<TabControllerTarget,
           URLDropTargetController,
           GTMWindowSheetControllerDelegate,
           TabContentsControllerDelegate> {
 @protected
  
  BOOL verticalLayout_;

 @private
  scoped_nsobject<TabStripView> tabStripView_;
  NSView* switchView_;  
  scoped_nsobject<NSView> dragBlockingView_;  
  NewTabButton* newTabButton_;  
  ProfileMenuButton* profileMenuButton_;  
  BOOL hasUpdatedProfileMenuButtonXOffset_;

  
  scoped_nsobject<CrTrackingArea> newTabTrackingArea_;
  scoped_ptr<TabStripModelObserverBridge> bridge_;
  Browser* browser_;  
  TabStripModel* tabStripModel_;  
  
  id<TabStripControllerDelegate> delegate_;  

  
  
  BOOL newTabButtonShowingHoverImage_;

  
  
  
  
  
  
  
  
  scoped_nsobject<NSMutableArray> tabContentsArray_;
  
  
  
  scoped_nsobject<NSMutableArray> tabArray_;

  
  scoped_nsobject<NSMutableSet> closingControllers_;

  
  TabView* placeholderTab_;  
  NSRect placeholderFrame_;  
  CGFloat placeholderStretchiness_; 
  NSRect droppedTabFrame_;  
  
  
  
  scoped_nsobject<NSMutableDictionary> targetFrames_;
  NSRect newTabTargetFrame_;
  
  BOOL forceNewTabButtonHidden_;
  
  
  
  BOOL initialLayoutComplete_;

  
  
  
  
  float availableResizeWidth_;
  
  
  scoped_nsobject<CrTrackingArea> trackingArea_;
  TabView* hoveredTab_;  

  
  
  scoped_nsobject<NSMutableArray> permanentSubviews_;

  
  scoped_nsobject<NSImage> defaultFavicon_;

  
  
  CGFloat indentForControls_;

  
  scoped_nsobject<GTMWindowSheetController> sheetController_;

  
  BOOL mouseInside_;

  
  scoped_ptr<TabStripControllerInternal::NotificationBridge>
      notificationBridge_;
}

@property(nonatomic) CGFloat indentForControls;

- (id)initWithView:(TabStripView*)view
        switchView:(NSView*)switchView
           browser:(Browser*)browser
          delegate:(id<TabStripControllerDelegate>)delegate;

- (NSView*)selectedTabView;

- (void)setFrameOfSelectedTab:(NSRect)frame;

- (void)moveTabFromIndex:(NSInteger)from;

- (void)dropTabContents:(TabContentsWrapper*)contents
              withFrame:(NSRect)frame
            asPinnedTab:(BOOL)pinned;

- (NSInteger)modelIndexForTabView:(NSView*)view;

- (NSView*)viewAtIndex:(NSUInteger)index;

- (NSUInteger)viewsCount;

- (void)insertPlaceholderForTab:(TabView*)tab
                          frame:(NSRect)frame
                  yStretchiness:(CGFloat)yStretchiness;

- (BOOL)isDragSessionActive;

- (BOOL)isTabFullyVisible:(TabView*)tab;

- (void)showNewTabButton:(BOOL)show;

- (void)layoutTabs;

- (BOOL)inRapidClosureMode;

- (BOOL)tabDraggingAllowed;

+ (CGFloat)defaultTabHeight;

+ (CGFloat)defaultIndentForControls;

- (GTMWindowSheetController*)sheetController;

- (void)destroySheetController;

- (TabContentsController*)activeTabContentsController;

  
  
- (void)attachConstrainedWindow:(ConstrainedWindowMac*)window;
- (void)removeConstrainedWindow:(ConstrainedWindowMac*)window;

@end

extern NSString* const kTabStripNumberOfTabsChanged;

#endif  
