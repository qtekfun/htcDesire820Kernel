// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_BOOT_SPLASH_SCREEN_CHROMEOS_H_
#define ASH_WM_BOOT_SPLASH_SCREEN_CHROMEOS_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"

namespace aura {
class RootWindow;
}

namespace ui {
class Layer;
class LayerDelegate;
}

namespace ash {
namespace internal {

class BootSplashScreen {
 public:
  explicit BootSplashScreen(aura::RootWindow* root_window);
  ~BootSplashScreen();

  
  void StartHideAnimation(base::TimeDelta duration);

 private:
  class CopyHostContentLayerDelegate;

  
  scoped_ptr<CopyHostContentLayerDelegate> layer_delegate_;

  scoped_ptr<ui::Layer> layer_;

  DISALLOW_COPY_AND_ASSIGN(BootSplashScreen);
};

}  
}  

#endif  
