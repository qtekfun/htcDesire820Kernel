// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_DISPLAY_OUTPUT_CONFIGURATOR_ANIMATION_H_
#define ASH_DISPLAY_OUTPUT_CONFIGURATOR_ANIMATION_H_

#include <map>

#include "ash/ash_export.h"
#include "base/callback.h"
#include "base/timer/timer.h"
#include "chromeos/display/output_configurator.h"

namespace aura {
class RootWindow;
class Window;
}  

namespace ui {
class Layer;
}  

namespace ash {
namespace internal {

class ASH_EXPORT OutputConfiguratorAnimation
    : public chromeos::OutputConfigurator::Observer {
 public:
  OutputConfiguratorAnimation();
  virtual ~OutputConfiguratorAnimation();

  
  
  void StartFadeOutAnimation(base::Closure callback);

  
  
  void StartFadeInAnimation();

 protected:
  
  virtual void OnDisplayModeChanged(
      const std::vector<chromeos::OutputConfigurator::OutputSnapshot>& outputs)
      OVERRIDE;
  virtual void OnDisplayModeChangeFailed(
      chromeos::OutputState failed_new_state) OVERRIDE;

 private:
  
  
  
  void ClearHidingLayers();

  std::map<aura::Window*, ui::Layer*> hiding_layers_;
  scoped_ptr<base::OneShotTimer<OutputConfiguratorAnimation> > timer_;

  DISALLOW_COPY_AND_ASSIGN(OutputConfiguratorAnimation);
};

}  
}  

#endif  
