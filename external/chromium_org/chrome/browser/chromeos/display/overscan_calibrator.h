// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DISPLAY_OVERSCAN_CALIBRATOR_H_
#define CHROME_BROWSER_CHROMEOS_DISPLAY_OVERSCAN_CALIBRATOR_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ui/compositor/layer_delegate.h"
#include "ui/gfx/display.h"
#include "ui/gfx/insets.h"
#include "ui/gfx/rect.h"

namespace ui {
class Layer;
}

namespace chromeos {

class OverscanCalibrator : public ui::LayerDelegate {
 public:
  OverscanCalibrator(const gfx::Display& target_display,
                     const gfx::Insets& initial_insets);
  virtual ~OverscanCalibrator();

  
  void Commit();

  
  
  
  void Reset();

  
  void UpdateInsets(const gfx::Insets& insets);

  const gfx::Insets& insets() const { return insets_; }

 private:
  
  virtual void OnPaintLayer(gfx::Canvas* canvas) OVERRIDE;
  virtual void OnDeviceScaleFactorChanged(float device_scale_factor) OVERRIDE;
  virtual base::Closure PrepareForLayerBoundsChange() OVERRIDE;

  
  const gfx::Display display_;

  
  gfx::Insets insets_;

  
  gfx::Insets initial_insets_;

  
  bool committed_;

  
  scoped_ptr<ui::Layer> calibration_layer_;

  DISALLOW_COPY_AND_ASSIGN(OverscanCalibrator);
};

}  

#endif  
