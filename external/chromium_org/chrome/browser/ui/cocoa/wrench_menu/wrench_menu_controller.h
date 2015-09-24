// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_WRENCH_MENU_WRENCH_MENU_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_WRENCH_MENU_WRENCH_MENU_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"
#import "ui/base/cocoa/menu_controller.h"

class BookmarkMenuBridge;
class Browser;
@class MenuTrackedRootView;
class RecentTabsMenuModelDelegate;
@class ToolbarController;
@class WrenchMenuButtonViewController;
class WrenchMenuModel;

namespace WrenchMenuControllerInternal {
class AcceleratorDelegate;
class ZoomLevelObserver;
}  

@interface WrenchMenuController : MenuController<NSMenuDelegate> {
 @private
  
  scoped_ptr<WrenchMenuControllerInternal::AcceleratorDelegate>
      acceleratorDelegate_;

  
  scoped_ptr<WrenchMenuModel> wrenchMenuModel_;

  
  
  scoped_ptr<RecentTabsMenuModelDelegate> recentTabsMenuModelDelegate_;

  
  
  base::scoped_nsobject<WrenchMenuButtonViewController> buttonViewController_;

  
  Browser* browser_;  

  
  scoped_ptr<BookmarkMenuBridge> bookmarkMenuBridge_;

  
  scoped_ptr<WrenchMenuControllerInternal::ZoomLevelObserver> observer_;
}

- (id)initWithBrowser:(Browser*)browser;

- (IBAction)dispatchWrenchMenuCommand:(id)sender;

- (WrenchMenuModel*)wrenchMenuModel;

- (void)updateRecentTabsSubmenu;

@end


@interface WrenchMenuButtonViewController : NSViewController {
 @private
  WrenchMenuController* controller_;

  MenuTrackedRootView* editItem_;
  NSButton* editCut_;
  NSButton* editCopy_;
  NSButton* editPaste_;

  MenuTrackedRootView* zoomItem_;
  NSButton* zoomPlus_;
  NSButton* zoomDisplay_;
  NSButton* zoomMinus_;
  NSButton* zoomFullScreen_;
}

@property(assign, nonatomic) IBOutlet MenuTrackedRootView* editItem;
@property(assign, nonatomic) IBOutlet NSButton* editCut;
@property(assign, nonatomic) IBOutlet NSButton* editCopy;
@property(assign, nonatomic) IBOutlet NSButton* editPaste;
@property(assign, nonatomic) IBOutlet MenuTrackedRootView* zoomItem;
@property(assign, nonatomic) IBOutlet NSButton* zoomPlus;
@property(assign, nonatomic) IBOutlet NSButton* zoomDisplay;
@property(assign, nonatomic) IBOutlet NSButton* zoomMinus;
@property(assign, nonatomic) IBOutlet NSButton* zoomFullScreen;

- (id)initWithController:(WrenchMenuController*)controller;
- (IBAction)dispatchWrenchMenuCommand:(id)sender;

@end

#endif  
