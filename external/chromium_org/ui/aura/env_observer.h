// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_ENV_OBSERVER_H_
#define UI_AURA_ENV_OBSERVER_H_

#include "ui/aura/aura_export.h"

namespace aura {

class RootWindow;
class Window;

class AURA_EXPORT EnvObserver {
 public:
  
  virtual void OnWindowInitialized(Window* window) = 0;

 
 virtual void OnRootWindowInitialized(RootWindow* root_window) {};

  
  virtual void OnRootWindowActivated(RootWindow* root_window) {}

  
  virtual void OnWillDestroyEnv() {}

 protected:
  virtual ~EnvObserver() {}
};

}  

#endif  
