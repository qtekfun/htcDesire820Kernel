// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_ROOT_WINDOW_SETTINGS_H_
#define ASH_ROOT_WINDOW_SETTINGS_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"

namespace aura {
class Window;
}

namespace ash {
namespace internal {

class RootWindowController;

struct RootWindowSettings {
  RootWindowSettings();

  
  int64 display_id;

  
  
  RootWindowController* controller;
};

RootWindowSettings* InitRootWindowSettings(aura::Window* root);

ASH_EXPORT RootWindowSettings* GetRootWindowSettings(aura::Window* root);

ASH_EXPORT const RootWindowSettings*
GetRootWindowSettings(const aura::Window* root);

}  
}  

#endif  
