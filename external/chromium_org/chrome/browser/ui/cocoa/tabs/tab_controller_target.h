// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABS_TAB_CONTROLLER_TARGET_H_
#define CHROME_BROWSER_UI_COCOA_TABS_TAB_CONTROLLER_TARGET_H_

#include "chrome/browser/ui/tabs/tab_menu_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"

@class TabController;
@protocol TabDraggingEventTarget;

@protocol TabControllerTarget
- (void)selectTab:(id)sender;
- (void)closeTab:(id)sender;

- (void)commandDispatch:(TabStripModel::ContextMenuCommand)command
          forController:(TabController*)controller;
- (BOOL)isCommandEnabled:(TabStripModel::ContextMenuCommand)command
           forController:(TabController*)controller;

- (ui::SimpleMenuModel*)contextMenuModelForController:(TabController*)controller
    menuDelegate:(ui::SimpleMenuModel::Delegate*)delegate;

- (id<TabDraggingEventTarget>)dragController;

@end

#endif  
