// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_ACTION_CONTEXT_MENU_H_
#define CHROME_BROWSER_UI_COCOA_EXTENSIONS_EXTENSION_ACTION_CONTEXT_MENU_H_
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/memory/scoped_nsobject.h"
#include "base/memory/scoped_ptr.h"

class AsyncUninstaller;
class DevmodeObserver;
class Extension;
class ExtensionAction;
class NotificationRegistrar;
class Profile;

namespace extension_action_context_menu {

class DevmodeObserver;
class ProfileObserverBridge;

}  

@interface ExtensionActionContextMenu : NSMenu {
 @private
  
  const Extension* extension_;

  
  ExtensionAction* action_;

  
  Profile* profile_;

  
  scoped_nsobject<NSMenuItem> inspectorItem_;

  
  scoped_ptr<extension_action_context_menu::DevmodeObserver> observer_;

  
  scoped_ptr<extension_action_context_menu::ProfileObserverBridge>
      profile_observer_;

  
  
  scoped_ptr<AsyncUninstaller> uninstaller_;
}

- (id)initWithExtension:(const Extension*)extension
                profile:(Profile*)profile
        extensionAction:(ExtensionAction*)action;

- (void)updateInspectorItem;

- (void)invalidateProfile;

@end

typedef ExtensionActionContextMenu ExtensionActionContextMenuMac;

#endif  
