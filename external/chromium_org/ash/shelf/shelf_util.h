// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_UTIL_H_
#define ASH_SHELF_SHELF_UTIL_H_

#include "ash/ash_export.h"
#include "ash/launcher/launcher_types.h"
#include "ui/aura/window.h"

namespace aura {
class Window;
}

namespace ash {

extern const aura::WindowProperty<ash::LauncherItemDetails*>* const
    kLauncherItemDetailsKey;

ASH_EXPORT void SetLauncherIDForWindow(LauncherID id, aura::Window* window);

ASH_EXPORT LauncherID GetLauncherIDForWindow(aura::Window* window);

ASH_EXPORT void SetLauncherItemDetailsForWindow(
    aura::Window* window,
    const LauncherItemDetails& details);

ASH_EXPORT void ClearLauncherItemDetailsForWindow(aura::Window* window);

ASH_EXPORT const LauncherItemDetails* GetLauncherItemDetailsForWindow(
    aura::Window* window);

}  

#endif  
