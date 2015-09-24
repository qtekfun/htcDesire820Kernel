// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_MENU_BUTTON_H_
#define CHROME_BROWSER_UI_COCOA_MENU_BUTTON_H_

#import <Cocoa/Cocoa.h>

#include "base/mac/scoped_nsobject.h"
#import "chrome/browser/ui/cocoa/toolbar/toolbar_button.h"

@interface MenuButton : ToolbarButton {
 @private
  base::scoped_nsobject<NSMenu> attachedMenu_;
  BOOL openMenuOnClick_;
  BOOL openMenuOnRightClick_;
  base::scoped_nsobject<NSPopUpButtonCell> popUpCell_;
}

@property(retain, nonatomic) IBOutlet NSMenu* attachedMenu;

@property(assign, nonatomic) BOOL openMenuOnClick;

@property(assign, nonatomic) BOOL openMenuOnRightClick;

- (NSRect)menuRect;

@end  

@interface MenuButton (Protected)
- (void)configureCell;
@end

#endif  
