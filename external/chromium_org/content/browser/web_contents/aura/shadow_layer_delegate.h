// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_WEB_CONTENTS_AURA_SHADOW_LAYER_DELEGATE_H_
#define CONTENT_BROWSER_WEB_CONTENTS_AURA_SHADOW_LAYER_DELEGATE_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "ui/compositor/layer_delegate.h"

namespace aura {
class Window;
}

namespace ui {
class Layer;
}

namespace content {

class ShadowLayerDelegate : public ui::LayerDelegate {
 public:
  explicit ShadowLayerDelegate(ui::Layer* shadow_for);
  virtual ~ShadowLayerDelegate();

  
  
  ui::Layer* layer() { return layer_.get(); }

 private:
  
  virtual void OnPaintLayer(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE;
  virtual base::Closure PrepareForLayerBoundsChange() OVERRIDE;

  scoped_ptr<ui::Layer> layer_;

  DISALLOW_COPY_AND_ASSIGN(ShadowLayerDelegate);
};

}  

#endif  
