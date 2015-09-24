// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_ACTION_CONTEXT_MENU_CONTROLLER_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_ACTION_CONTEXT_MENU_CONTROLLER_H_

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_ptr.h"

class AsyncUninstaller;
class Browser;
class ExtensionAction;

namespace extensions {
class Extension;
}

@interface ExtensionActionContextMenuController : NSObject {
 @private
  
  const extensions::Extension* extension_;

  
  ExtensionAction* action_;

  
  Browser* browser_;

  
  
  scoped_ptr<AsyncUninstaller> uninstaller_;
}

- (id)initWithExtension:(const extensions::Extension*)extension
                browser:(Browser*)browser
        extensionAction:(ExtensionAction*)action;

- (void)populateMenu:(NSMenu*)menu;

@end

#endif  
