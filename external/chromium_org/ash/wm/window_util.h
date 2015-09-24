// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WINDOW_UTIL_H_
#define ASH_WM_WINDOW_UTIL_H_

#include "ash/ash_export.h"
#include "base/compiler_specific.h"
#include "ui/base/ui_base_types.h"

namespace aura {
class Window;
}

namespace gfx {
class Rect;
}

namespace ui {
class Event;
}

namespace ash {
const int kMinimumOnScreenArea = 10;

namespace wm {

ASH_EXPORT void ActivateWindow(aura::Window* window);
ASH_EXPORT void DeactivateWindow(aura::Window* window);
ASH_EXPORT bool IsActiveWindow(aura::Window* window);
ASH_EXPORT aura::Window* GetActiveWindow();
ASH_EXPORT bool CanActivateWindow(aura::Window* window);

ASH_EXPORT aura::Window* GetActivatableWindow(aura::Window* window);

ASH_EXPORT bool IsWindowMinimized(aura::Window* window);

ASH_EXPORT void CenterWindow(aura::Window* window);

ASH_EXPORT void AdjustBoundsToEnsureMinimumWindowVisibility(
    const gfx::Rect& visible_area,
    gfx::Rect* bounds);

ASH_EXPORT void AdjustBoundsToEnsureWindowVisibility(
    const gfx::Rect& visible_area,
    int min_width,
    int min_height,
    gfx::Rect* bounds);

ASH_EXPORT bool MoveWindowToEventRoot(aura::Window* window,
                                      const ui::Event& event);

void ReparentChildWithTransientChildren(aura::Window* child,
                                        aura::Window* old_parent,
                                        aura::Window* new_parent);

void ReparentTransientChildrenOfChild(aura::Window* child,
                                      aura::Window* old_parent,
                                      aura::Window* new_parent);

}  
}  

#endif  
