// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABS_TAB_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TABS_TAB_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>
#import "chrome/browser/ui/cocoa/hover_close_button.h"
#include "chrome/browser/ui/tabs/tab_menu_model.h"
#include "googleurl/src/gurl.h"

enum TabLoadingState {
  kTabDone,
  kTabLoading,
  kTabWaiting,
  kTabCrashed,
};

@class MenuController;
namespace TabControllerInternal {
class MenuDelegate;
}
@class TabView;
@protocol TabControllerTarget;


@interface TabController : NSViewController {
 @private
  IBOutlet NSView* iconView_;
  IBOutlet NSTextField* titleView_;
  IBOutlet HoverCloseButton* closeButton_;

  NSRect originalIconFrame_;  
  BOOL isIconShowing_;  

  BOOL app_;
  BOOL mini_;
  BOOL pinned_;
  BOOL selected_;
  GURL url_;
  TabLoadingState loadingState_;
  CGFloat iconTitleXOffset_;  
  id<TabControllerTarget> target_;  
  SEL action_;  
  scoped_ptr<TabMenuModel> contextMenuModel_;
  scoped_ptr<TabControllerInternal::MenuDelegate> contextMenuDelegate_;
  scoped_nsobject<MenuController> contextMenuController_;
}

@property(assign, nonatomic) TabLoadingState loadingState;

@property(assign, nonatomic) SEL action;
@property(assign, nonatomic) BOOL app;
@property(assign, nonatomic) BOOL mini;
@property(assign, nonatomic) BOOL pinned;
@property(assign, nonatomic) BOOL selected;
@property(assign, nonatomic) id target;
@property(assign, nonatomic) GURL url;
@property(assign, nonatomic) NSView* iconView;
@property(assign, nonatomic) NSTextField* titleView;
@property(assign, nonatomic) HoverCloseButton* closeButton;

+ (CGFloat)minTabWidth;
+ (CGFloat)maxTabWidth;
+ (CGFloat)minSelectedTabWidth;
+ (CGFloat)miniTabWidth;
+ (CGFloat)appTabWidth;

- (TabView*)tabView;

- (IBAction)closeTab:(id)sender;

- (void)setIconView:(NSView*)iconView;
- (NSView*)iconView;

- (BOOL)inRapidClosureMode;

- (void)updateVisibility;

- (void)updateTitleColor;
@end

@interface TabController(TestingAPI)
- (NSString*)toolTip;
- (int)iconCapacity;
- (BOOL)shouldShowIcon;
- (BOOL)shouldShowCloseButton;
@end  

#endif  
