// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABS_TAB_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_TABS_TAB_CONTROLLER_H_

#import <Cocoa/Cocoa.h>
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/hover_close_button.h"
#import "chrome/browser/ui/cocoa/tabs/tab_strip_drag_controller.h"
#include "chrome/browser/ui/tabs/tab_menu_model.h"
#include "url/gurl.h"

enum TabLoadingState {
  kTabDone,
  kTabLoading,
  kTabWaiting,
  kTabCrashed,
};

@class GTMFadeTruncatingTextFieldCell;
@class MediaIndicatorView;
@class MenuController;
namespace TabControllerInternal {
class MenuDelegate;
}
@class TabView;
@protocol TabControllerTarget;


@interface TabController : NSViewController<TabDraggingEventTarget> {
 @private
  base::scoped_nsobject<NSView> iconView_;
  base::scoped_nsobject<NSTextField> titleView_;
  GTMFadeTruncatingTextFieldCell* titleViewCell_;  
  base::scoped_nsobject<MediaIndicatorView> mediaIndicatorView_;
  base::scoped_nsobject<HoverCloseButton> closeButton_;

  NSRect originalIconFrame_;  
  BOOL isIconShowing_;  

  BOOL app_;
  BOOL mini_;
  BOOL pinned_;
  BOOL active_;
  BOOL selected_;
  GURL url_;
  TabLoadingState loadingState_;
  id<TabControllerTarget> target_;  
  SEL action_;  
  scoped_ptr<ui::SimpleMenuModel> contextMenuModel_;
  scoped_ptr<TabControllerInternal::MenuDelegate> contextMenuDelegate_;
  base::scoped_nsobject<MenuController> contextMenuController_;
}

@property(assign, nonatomic) TabLoadingState loadingState;

@property(assign, nonatomic) SEL action;
@property(assign, nonatomic) BOOL app;
@property(assign, nonatomic) BOOL mini;
@property(assign, nonatomic) BOOL pinned;
@property(assign, nonatomic) NSString* toolTip;
@property(assign, nonatomic) BOOL active;
@property(assign, nonatomic) BOOL selected;
@property(assign, nonatomic) id target;
@property(assign, nonatomic) GURL url;
@property(assign, nonatomic) NSView* iconView;
@property(readonly, nonatomic) NSTextField* titleView;
@property(assign, nonatomic) MediaIndicatorView* mediaIndicatorView;
@property(readonly, nonatomic) HoverCloseButton* closeButton;

+ (CGFloat)minTabWidth;
+ (CGFloat)maxTabWidth;
+ (CGFloat)minSelectedTabWidth;
+ (CGFloat)miniTabWidth;
+ (CGFloat)appTabWidth;

- (TabView*)tabView;

- (void)closeTab:(id)sender;

- (void)selectTab:(id)sender;

- (BOOL)inRapidClosureMode;

- (void)updateVisibility;

- (void)updateTitleColor;
@end

@interface TabController(TestingAPI)
- (int)iconCapacity;
- (BOOL)shouldShowIcon;
- (BOOL)shouldShowMediaIndicator;
- (BOOL)shouldShowCloseButton;
@end  

#endif  
