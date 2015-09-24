// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_ACTIVATION_CLIENT_H_
#define UI_AURA_CLIENT_ACTIVATION_CLIENT_H_

#include "ui/aura/aura_export.h"

namespace ui {
class Event;
}

namespace aura {
class Window;

namespace client {
class ActivationChangeObserver;

class AURA_EXPORT ActivationClient {
 public:
  
  virtual void AddObserver(ActivationChangeObserver* observer) = 0;
  virtual void RemoveObserver(ActivationChangeObserver* observer) = 0;

  
  virtual void ActivateWindow(Window* window) = 0;

  
  
  virtual void DeactivateWindow(Window* window) = 0;

  
  virtual Window* GetActiveWindow() = 0;

  
  
  
  
  
  virtual Window* GetActivatableWindow(Window* window) = 0;

  
  virtual Window* GetToplevelWindow(Window* window) = 0;

  
  
  virtual bool OnWillFocusWindow(Window* window, const ui::Event* event) = 0;

  
  
  virtual bool CanActivateWindow(Window* window) const = 0;

 protected:
  virtual ~ActivationClient() {}
};

AURA_EXPORT void SetActivationClient(Window* root_window,
                                     ActivationClient* client);
AURA_EXPORT ActivationClient* GetActivationClient(Window* root_window);

AURA_EXPORT void SetHideOnDeactivate(Window* window, bool hide_on_deactivate);
AURA_EXPORT bool GetHideOnDeactivate(Window* window);

}  
}  

#endif  
