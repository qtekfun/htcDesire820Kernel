// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_RESIZE_SHADOW_CONTROLLER_H_
#define ASH_WM_RESIZE_SHADOW_CONTROLLER_H_

#include <map>

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "ui/aura/window_observer.h"

namespace aura {
class Window;
}

namespace ash {
namespace internal {

class ResizeShadow;

class ASH_EXPORT ResizeShadowController : public aura::WindowObserver {
 public:
  ResizeShadowController();
  virtual ~ResizeShadowController();

  
  void ShowShadow(aura::Window* window, int hit_test);

  
  void HideShadow(aura::Window* window);

  ResizeShadow* GetShadowForWindowForTest(aura::Window* window);

  
  virtual void OnWindowBoundsChanged(
      aura::Window* window,
      const gfx::Rect& old_bounds,
      const gfx::Rect& new_bounds) OVERRIDE;
  virtual void OnWindowDestroyed(aura::Window* window) OVERRIDE;

 private:
  typedef std::map<aura::Window*, linked_ptr<ResizeShadow> > WindowShadowMap;

  
  
  ResizeShadow* CreateShadow(aura::Window* window);

  
  ResizeShadow* GetShadowForWindow(aura::Window* window);

  WindowShadowMap window_shadows_;

  DISALLOW_COPY_AND_ASSIGN(ResizeShadowController);
};

}  
}  

#endif  
