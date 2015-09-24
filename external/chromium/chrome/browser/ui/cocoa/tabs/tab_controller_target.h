// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_TABS_TAB_CONTROLLER_TARGET_H_
#define CHROME_BROWSER_UI_COCOA_TABS_TAB_CONTROLLER_TARGET_H_
#pragma once

#include "chrome/browser/tabs/tab_strip_model.h"

@class TabController;

@protocol TabControllerTarget
- (void)selectTab:(id)sender;
- (void)closeTab:(id)sender;

- (void)commandDispatch:(TabStripModel::ContextMenuCommand)command
          forController:(TabController*)controller;
- (BOOL)isCommandEnabled:(TabStripModel::ContextMenuCommand)command
           forController:(TabController*)controller;
@end

#endif  
