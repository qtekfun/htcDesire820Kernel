// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_FOCUS_CLIENT_H_
#define UI_AURA_CLIENT_FOCUS_CLIENT_H_

#include "ui/aura/aura_export.h"

namespace ui {
class Event;
}

namespace aura {
class Window;
namespace client {
class FocusChangeObserver;

class AURA_EXPORT FocusClient {
 public:
  virtual ~FocusClient() {}

  virtual void AddObserver(FocusChangeObserver* observer) = 0;
  virtual void RemoveObserver(FocusChangeObserver* observer) = 0;

  
  virtual void FocusWindow(Window* window) = 0;

  
  
  virtual void ResetFocusWithinActiveWindow(Window* window) = 0;

  
  virtual Window* GetFocusedWindow() = 0;
};

AURA_EXPORT void SetFocusClient(Window* root_window, FocusClient* client);
AURA_EXPORT FocusClient* GetFocusClient(Window* window);
AURA_EXPORT FocusClient* GetFocusClient(const Window* window);

}  
}  

#endif  
