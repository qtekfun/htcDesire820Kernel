// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_DELEGATE_H_
#define ASH_SHELF_SHELF_DELEGATE_H_

#include "ash/ash_export.h"
#include "ash/launcher/launcher_types.h"

namespace ash {
class Launcher;

class ASH_EXPORT ShelfDelegate {
 public:
  
  virtual ~ShelfDelegate() {}

  
  
  virtual void OnLauncherCreated(Launcher* launcher) = 0;

  
  
  virtual void OnLauncherDestroyed(Launcher* launcher) = 0;

  
  virtual LauncherID GetLauncherIDForAppID(const std::string& app_id) = 0;

  
  virtual const std::string& GetAppIDForLauncherID(LauncherID id) = 0;

  
  
  
  virtual void PinAppWithID(const std::string& app_id) = 0;

  
  virtual bool IsAppPinned(const std::string& app_id) = 0;

  
  
  virtual bool CanPin() const = 0;

  
  virtual void UnpinAppWithID(const std::string& app_id) = 0;
};

}  

#endif  
