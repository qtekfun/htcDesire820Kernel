// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_UI_FOCUS_RING_LAYER_H_
#define CHROME_BROWSER_CHROMEOS_UI_FOCUS_RING_LAYER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/compositor/layer_delegate.h"
#include "ui/gfx/rect.h"

namespace aura {
class Window;
}

namespace ui {
class Layer;
}

namespace views {
class View;
}

namespace chromeos {

class FocusRingLayer : public ui::LayerDelegate {
 public:
  FocusRingLayer();
  virtual ~FocusRingLayer();

  
  void Update();

  
  void SetForView(views::View* view);

 private:
  
  virtual void OnPaintLayer(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE;
  virtual base::Closure PrepareForLayerBoundsChange() OVERRIDE;

  
  aura::Window* window_;

  
  aura::Window* root_window_;

  
  gfx::Rect focus_ring_;

  
  scoped_ptr<ui::Layer> layer_;

  DISALLOW_COPY_AND_ASSIGN(FocusRingLayer);
};

}  

#endif  
