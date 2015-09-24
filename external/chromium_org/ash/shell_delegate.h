// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELL_DELEGATE_H_
#define ASH_SHELL_DELEGATE_H_

#include <string>

#include "ash/ash_export.h"
#include "ash/shell.h"
#include "base/callback.h"
#include "base/strings/string16.h"

namespace app_list {
class AppListViewDelegate;
}

namespace aura {
class RootWindow;
class Window;
namespace client {
class UserActionClient;
}
}

namespace content {
class BrowserContext;
}

namespace ui {
class MenuModel;
}

namespace views {
class Widget;
}

namespace keyboard {
class KeyboardControllerProxy;
}

namespace ash {

class AccessibilityDelegate;
class CapsLockDelegate;
class MediaDelegate;
class NewWindowDelegate;
class RootWindowHostFactory;
class SessionStateDelegate;
class ShelfDelegate;
class ShelfModel;
class SystemTrayDelegate;
class UserWallpaperDelegate;
struct LauncherItem;

class ASH_EXPORT ShellDelegate {
 public:
  
  virtual ~ShellDelegate() {}

  
  
  
  virtual bool IsFirstRunAfterBoot() const = 0;

  
  virtual bool IsMultiProfilesEnabled() const = 0;

  
  
  virtual bool IsIncognitoAllowed() const = 0;

  
  virtual bool IsRunningInForcedAppMode() const = 0;

  
  
  virtual void PreInit() = 0;

  
  virtual void Shutdown() = 0;

  
  virtual void Exit() = 0;

  
  virtual keyboard::KeyboardControllerProxy*
      CreateKeyboardControllerProxy() = 0;

  
  
  virtual content::BrowserContext* GetActiveBrowserContext() = 0;

  
  
  virtual app_list::AppListViewDelegate* CreateAppListViewDelegate() = 0;

  
  
  virtual ShelfDelegate* CreateShelfDelegate(ShelfModel* model) = 0;

  
  virtual SystemTrayDelegate* CreateSystemTrayDelegate() = 0;

  
  virtual UserWallpaperDelegate* CreateUserWallpaperDelegate() = 0;

  
  virtual CapsLockDelegate* CreateCapsLockDelegate() = 0;

  
  virtual SessionStateDelegate* CreateSessionStateDelegate() = 0;

  
  virtual AccessibilityDelegate* CreateAccessibilityDelegate() = 0;

  
  virtual NewWindowDelegate* CreateNewWindowDelegate() = 0;

  
  virtual MediaDelegate* CreateMediaDelegate() = 0;

  
  virtual aura::client::UserActionClient* CreateUserActionClient() = 0;

  
  virtual ui::MenuModel* CreateContextMenu(aura::Window* root_window) = 0;

  
  
  virtual RootWindowHostFactory* CreateRootWindowHostFactory() = 0;

  
  virtual base::string16 GetProductName() const = 0;
};

}  

#endif  
