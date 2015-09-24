// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_SYSTEM_BACKGROUND_CONTROLLER_H_
#define ASH_WM_SYSTEM_BACKGROUND_CONTROLLER_H_

#include <string>

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/aura/window_observer.h"

namespace ui {
class Layer;
}

namespace ash {
namespace internal {

class SystemBackgroundController : public aura::WindowObserver {
 public:
  SystemBackgroundController(aura::Window* root_window, SkColor color);
  virtual ~SystemBackgroundController();

  void SetColor(SkColor color);

  
  virtual void OnWindowBoundsChanged(aura::Window* root,
                                     const gfx::Rect& old_bounds,
                                     const gfx::Rect& new_bounds) OVERRIDE;

 private:
  class HostContentLayerDelegate;

  aura::Window* root_window_;  

  scoped_ptr<ui::Layer> layer_;

  DISALLOW_COPY_AND_ASSIGN(SystemBackgroundController);
};

}  
}  

#endif  
