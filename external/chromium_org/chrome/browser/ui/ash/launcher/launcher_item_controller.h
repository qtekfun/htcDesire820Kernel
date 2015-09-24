// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ASH_LAUNCHER_LAUNCHER_ITEM_CONTROLLER_H_
#define CHROME_BROWSER_UI_ASH_LAUNCHER_LAUNCHER_ITEM_CONTROLLER_H_

#include "ash/launcher/launcher_types.h"
#include "ash/shelf/shelf_item_delegate.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_vector.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/ash/launcher/chrome_launcher_types.h"
#include "ui/events/event.h"

class ChromeLauncherController;
class ChromeLauncherAppMenuItem;

typedef ScopedVector<ChromeLauncherAppMenuItem> ChromeLauncherAppMenuItems;

namespace aura {
class Window;
}

namespace content {
class WebContents;
}

class LauncherItemController : public ash::ShelfItemDelegate {
 public:
  enum Type {
    TYPE_APP,
    TYPE_APP_PANEL,
    TYPE_SHORTCUT,
    TYPE_WINDOWED_APP
  };

  LauncherItemController(Type type,
                         const std::string& app_id,
                         ChromeLauncherController* launcher_controller);
  virtual ~LauncherItemController();

  Type type() const { return type_; }
  ash::LauncherID launcher_id() const { return launcher_id_; }
  void set_launcher_id(ash::LauncherID id) { launcher_id_ = id; }
  virtual const std::string& app_id() const;
  ChromeLauncherController* launcher_controller() const {
    return launcher_controller_;
  }

  
  void lock() { locked_++; }
  void unlock() {
    DCHECK(locked_);
    locked_--;
  }
  bool locked() { return locked_ > 0; }

  bool image_set_by_controller() const { return image_set_by_controller_; }
  void set_image_set_by_controller(bool image_set_by_controller) {
    image_set_by_controller_ = image_set_by_controller;
  }

  
  virtual bool IsOpen() const = 0;

  
  virtual bool IsVisible() const = 0;

  
  virtual void Launch(ash::LaunchSource source, int event_flags) = 0;

  
  
  
  virtual bool Activate(ash::LaunchSource source) = 0;

  
  virtual void Close() = 0;

  
  virtual ChromeLauncherAppMenuItems GetApplicationList(int event_flags) = 0;

  
  ash::LauncherItemType GetLauncherItemType() const;

 protected:
  
  
  base::string16 GetAppTitle() const;

 private:
  const Type type_;
  
  const std::string app_id_;
  ash::LauncherID launcher_id_;
  ChromeLauncherController* launcher_controller_;

  
  
  
  int locked_;

  
  bool image_set_by_controller_;

  DISALLOW_COPY_AND_ASSIGN(LauncherItemController);
};

#endif  
