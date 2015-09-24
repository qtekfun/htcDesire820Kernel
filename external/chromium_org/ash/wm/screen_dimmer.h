// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_SCREEN_DIMMER_H_
#define ASH_WM_SCREEN_DIMMER_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/window_observer.h"

namespace ui {
class Layer;
}

namespace ash {
namespace internal {

class ASH_EXPORT ScreenDimmer : public aura::WindowObserver {
 public:
  class TestApi {
   public:
    explicit TestApi(ScreenDimmer* dimmer) : dimmer_(dimmer) {}

    ui::Layer* layer() { return dimmer_->dimming_layer_.get(); }

   private:
    ScreenDimmer* dimmer_;  

    DISALLOW_COPY_AND_ASSIGN(TestApi);
  };

  explicit ScreenDimmer(aura::Window* root_window);
  virtual ~ScreenDimmer();

  
  void SetDimming(bool should_dim);

  
  virtual void OnWindowBoundsChanged(aura::Window* root_window,
                                     const gfx::Rect& old_bounds,
                                     const gfx::Rect& new_bounds) OVERRIDE;

 private:
  friend class TestApi;

  aura::Window* root_window_;

  
  
  scoped_ptr<ui::Layer> dimming_layer_;

  
  bool currently_dimming_;

  DISALLOW_COPY_AND_ASSIGN(ScreenDimmer);
};

}  
}  

#endif  
