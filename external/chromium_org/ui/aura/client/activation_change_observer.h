// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_ACTIVATION_CHANGE_OBSERVER_H_
#define UI_AURA_CLIENT_ACTIVATION_CHANGE_OBSERVER_H_

#include "ui/aura/aura_export.h"

namespace aura {
class Window;

namespace client {

class AURA_EXPORT ActivationChangeObserver {
 public:
  
  
  
  
  virtual void OnWindowActivated(Window* gained_active,
                                 Window* lost_active) = 0;

  
  
  
  virtual void OnAttemptToReactivateWindow(aura::Window* request_active,
                                           aura::Window* actual_active) {}

 protected:
  virtual ~ActivationChangeObserver() {}
};

AURA_EXPORT void SetActivationChangeObserver(
    Window* window,
    ActivationChangeObserver* observer);
AURA_EXPORT ActivationChangeObserver* GetActivationChangeObserver(
    Window* window);

}  
}  

#endif  
