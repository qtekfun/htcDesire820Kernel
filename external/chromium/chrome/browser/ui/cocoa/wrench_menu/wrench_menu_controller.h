// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_WRENCH_MENU_WRENCH_MENU_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_WRENCH_MENU_WRENCH_MENU_CONTROLLER_H_
#pragma once

#import <Cocoa/Cocoa.h>

#import "base/mac/cocoa_protocols.h"
#include "base/memory/scoped_ptr.h"
#import "chrome/browser/ui/cocoa/menu_controller.h"

@class MenuTrackedRootView;
@class ToolbarController;
class WrenchMenuModel;

namespace WrenchMenuControllerInternal {
class ZoomLevelObserver;
}  

@interface WrenchMenuController : MenuController<NSMenuDelegate> {
  IBOutlet MenuTrackedRootView* editItem_;
  IBOutlet NSButton* editCut_;
  IBOutlet NSButton* editCopy_;
  IBOutlet NSButton* editPaste_;

  IBOutlet MenuTrackedRootView* zoomItem_;
  IBOutlet NSButton* zoomPlus_;
  IBOutlet NSButton* zoomDisplay_;
  IBOutlet NSButton* zoomMinus_;
  IBOutlet NSButton* zoomFullScreen_;

  scoped_ptr<WrenchMenuControllerInternal::ZoomLevelObserver> observer_;
}

- (id)init;

- (IBAction)dispatchWrenchMenuCommand:(id)sender;

- (WrenchMenuModel*)wrenchMenuModel;

@end


@interface WrenchMenuController (UnitTesting)
- (void)dispatchCommandInternal:(NSInteger)tag;
@end

#endif  
