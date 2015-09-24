// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_INFOBARS_EXTENSION_INFOBAR_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_INFOBARS_EXTENSION_INFOBAR_CONTROLLER_H_

#import "chrome/browser/ui/cocoa/infobars/infobar_controller.h"

#import <Cocoa/Cocoa.h>

#import "base/mac/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"

@class ExtensionActionContextMenuController;
class InfobarBridge;
@class MenuButton;

@interface ExtensionInfoBarController : InfoBarController<NSMenuDelegate> {
  
  NSView* extensionView_;

  
  
  base::scoped_nsobject<MenuButton> dropdownButton_;

  
  base::scoped_nsobject<
      ExtensionActionContextMenuController> contextMenuController_;

  
  scoped_ptr<InfobarBridge> bridge_;
}

@end

#endif  
