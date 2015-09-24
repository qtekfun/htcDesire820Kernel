// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_FOCUS_CHANGE_OBSERVER_H_
#define UI_AURA_CLIENT_FOCUS_CHANGE_OBSERVER_H_

#include "ui/aura/aura_export.h"

namespace aura {
class Window;
namespace client {

class AURA_EXPORT FocusChangeObserver {
 public:
  
  virtual void OnWindowFocused(Window* gained_focus, Window* lost_focus) = 0;

 protected:
  virtual ~FocusChangeObserver() {}
};

AURA_EXPORT FocusChangeObserver* GetFocusChangeObserver(Window* window);
AURA_EXPORT void SetFocusChangeObserver(
    Window* window,
    FocusChangeObserver* focus_change_observer);


}  
}  

#endif  
