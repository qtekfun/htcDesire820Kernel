// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABS_TAB_STRIP_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TABS_TAB_STRIP_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/tabs/tab_controller_target.h"
#import "chrome/browser/ui/cocoa/url_drop_target.h"
#include "chrome/browser/ui/tabs/hover_tab_selector.h"

@class CrTrackingArea;
@class NewTabButton;
@class TabContentsController;
@class TabView;
@class TabStripDragController;
@class TabStripView;

class Browser;
class TabStripModelObserverBridge;
class TabStripModel;

namespace content {
class WebContents;
}

@protocol TabStripControllerDelegate

- (void)onActivateTabWithContents:(content::WebContents*)contents;

- (void)onTabChanged:(TabStripModelObserver::TabChangeType)change
        withContents:(content::WebContents*)contents;

- (void)onTabDetachedWithContents:(content::WebContents*)contents;

@end

@interface TabStripController :
  NSObject<TabControllerTarget,
           URLDropTargetController> {
 @private
  base::scoped_nsobject<TabStripView> tabStripView_;
  NSView* switchView_;  
  base::scoped_nsobject<NSView> dragBlockingView_;  
                                                    
  NewTabButton* newTabButton_;  

  
  base::scoped_nsobject<TabStripDragController> dragController_;

  
  base::scoped_nsobject<CrTrackingArea> newTabTrackingArea_;
  scoped_ptr<TabStripModelObserverBridge> bridge_;
  Browser* browser_;  
  TabStripModel* tabStripModel_;  
  
  id<TabStripControllerDelegate> delegate_;  

  
  
  BOOL newTabButtonShowingHoverImage_;

  
  
  
  
  
  
  
  
  base::scoped_nsobject<NSMutableArray> tabContentsArray_;
  
  
  
  base::scoped_nsobject<NSMutableArray> tabArray_;

  
  base::scoped_nsobject<NSMutableSet> closingControllers_;

  
  TabView* placeholderTab_;  
  NSRect placeholderFrame_;  
  NSRect droppedTabFrame_;  
  
  
  
  base::scoped_nsobject<NSMutableDictionary> targetFrames_;
  NSRect newTabTargetFrame_;
  
  BOOL forceNewTabButtonHidden_;
  
  
  
  BOOL initialLayoutComplete_;

  
  
  
  
  float availableResizeWidth_;
  
  
  base::scoped_nsobject<CrTrackingArea> trackingArea_;
  TabView* hoveredTab_;  

  
  
  base::scoped_nsobject<NSMutableArray> permanentSubviews_;

  
  base::scoped_nsobject<NSImage> defaultFavicon_;

  
  
  CGFloat leftIndentForControls_;
  CGFloat rightIndentForControls_;

  
  BOOL mouseInside_;

  
  scoped_ptr<HoverTabSelector> hoverTabSelector_;
}

@property(nonatomic) CGFloat leftIndentForControls;
@property(nonatomic) CGFloat rightIndentForControls;

- (id)initWithView:(TabStripView*)view
        switchView:(NSView*)switchView
           browser:(Browser*)browser
          delegate:(id<TabStripControllerDelegate>)delegate;

- (TabStripModel*)tabStripModel;

- (NSView*)activeTabView;

- (void)setFrameOfActiveTab:(NSRect)frame;

- (void)moveTabFromIndex:(NSInteger)from;

- (void)dropWebContents:(content::WebContents*)contents
              withFrame:(NSRect)frame
            asPinnedTab:(BOOL)pinned;

- (NSInteger)modelIndexForTabView:(NSView*)view;

- (NSView*)viewAtIndex:(NSUInteger)index;

- (NSUInteger)viewsCount;

- (void)insertPlaceholderForTab:(TabView*)tab frame:(NSRect)frame;

- (BOOL)isDragSessionActive;

- (BOOL)isTabFullyVisible:(TabView*)tab;

- (void)showNewTabButton:(BOOL)show;

- (void)layoutTabs;
- (void)layoutTabsWithoutAnimation;

- (BOOL)inRapidClosureMode;

- (BOOL)tabDraggingAllowed;

+ (CGFloat)defaultTabHeight;

+ (CGFloat)defaultLeftIndentForControls;

- (TabContentsController*)activeTabContentsController;

@end

@interface TabStripController(TestingAPI)
- (void)setTabTitle:(TabController*)tab
       withContents:(content::WebContents*)contents;
@end

NSView* GetSheetParentViewForWebContents(content::WebContents* web_contents);

#endif  
