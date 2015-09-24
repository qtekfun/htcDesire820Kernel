// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_INFOBAR_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_INFOBAR_CONTROLLER_H_
#pragma once

#import "chrome/browser/ui/cocoa/infobars/infobar_controller.h"

#import <Cocoa/Cocoa.h>

#import "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"

@class ExtensionActionContextMenu;
class ExtensionInfoBarDelegate;
class InfobarBridge;
@class MenuButton;

@interface ExtensionInfoBarController : InfoBarController {
  
  NSView* extensionView_;

  
  NSWindow* window_;

  
  
  scoped_nsobject<MenuButton> dropdownButton_;

  
  scoped_nsobject<ExtensionActionContextMenu> contextMenu_;

  
  scoped_ptr<InfobarBridge> bridge_;
}

@end

#endif  
