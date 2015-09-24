// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_WINDOW_UTIL_H_
#define UI_VIEWS_COREWM_WINDOW_UTIL_H_

#include "base/compiler_specific.h"
#include "ui/views/views_export.h"

namespace aura {
class Window;
}

namespace ui {
class Layer;
}

namespace views {
namespace corewm {

VIEWS_EXPORT void ActivateWindow(aura::Window* window);
VIEWS_EXPORT void DeactivateWindow(aura::Window* window);
VIEWS_EXPORT bool IsActiveWindow(aura::Window* window);
VIEWS_EXPORT bool CanActivateWindow(aura::Window* window);

VIEWS_EXPORT aura::Window* GetActivatableWindow(aura::Window* window);

VIEWS_EXPORT aura::Window* GetToplevelWindow(aura::Window* window);

VIEWS_EXPORT void DeepDeleteLayers(ui::Layer* layer);

VIEWS_EXPORT ui::Layer* RecreateWindowLayers(aura::Window* window,
                                            bool set_bounds) WARN_UNUSED_RESULT;

}  
}  

#endif  
