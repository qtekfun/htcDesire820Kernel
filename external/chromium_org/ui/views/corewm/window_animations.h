// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_COREWM_WINDOW_ANIMATIONS_H_
#define UI_VIEWS_COREWM_WINDOW_ANIMATIONS_H_

#include <vector>

#include "ui/views/views_export.h"

namespace aura {
class Window;
}
namespace base {
class TimeDelta;
}
namespace gfx {
class Rect;
}
namespace ui {
class ImplicitAnimationObserver;
class Layer;
class LayerAnimationSequence;
}

namespace views {
namespace corewm {

enum WindowVisibilityAnimationType {
  WINDOW_VISIBILITY_ANIMATION_TYPE_DEFAULT = 0,     
                                                    
                                                    
  WINDOW_VISIBILITY_ANIMATION_TYPE_DROP,            
  WINDOW_VISIBILITY_ANIMATION_TYPE_VERTICAL,        
  WINDOW_VISIBILITY_ANIMATION_TYPE_FADE,            
  WINDOW_VISIBILITY_ANIMATION_TYPE_ROTATE,          

  
  WINDOW_VISIBILITY_ANIMATION_MAX
};

enum WindowAnimationType {
  WINDOW_ANIMATION_TYPE_BOUNCE = 0,  
};

enum WindowVisibilityAnimationTransition {
  ANIMATE_SHOW = 0x1,
  ANIMATE_HIDE = 0x2,
  ANIMATE_BOTH = ANIMATE_SHOW | ANIMATE_HIDE,
  ANIMATE_NONE = 0x4,
};

VIEWS_EXPORT void SetWindowVisibilityAnimationType(aura::Window* window,
                                                   int type);
VIEWS_EXPORT int GetWindowVisibilityAnimationType(aura::Window* window);

VIEWS_EXPORT void SetWindowVisibilityAnimationTransition(
    aura::Window* window,
    WindowVisibilityAnimationTransition transition);

VIEWS_EXPORT bool HasWindowVisibilityAnimationTransition(
    aura::Window* window,
    WindowVisibilityAnimationTransition transition);

VIEWS_EXPORT void SetWindowVisibilityAnimationDuration(
    aura::Window* window,
    const base::TimeDelta& duration);

VIEWS_EXPORT void SetWindowVisibilityAnimationVerticalPosition(
    aura::Window* window,
    float position);

VIEWS_EXPORT ui::ImplicitAnimationObserver* CreateHidingWindowAnimationObserver(
    aura::Window* window);

VIEWS_EXPORT bool AnimateOnChildWindowVisibilityChanged(aura::Window* window,
                                                        bool visible);
VIEWS_EXPORT bool AnimateWindow(aura::Window* window, WindowAnimationType type);

VIEWS_EXPORT bool WindowAnimationsDisabled(aura::Window* window);

}  
}  

#endif  
