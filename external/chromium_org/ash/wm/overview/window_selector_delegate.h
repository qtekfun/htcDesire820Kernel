// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_OVERVIEW_WINDOW_SELECTOR_DELEGATE_H_
#define ASH_WM_OVERVIEW_WINDOW_SELECTOR_DELEGATE_H_

#include "ash/ash_export.h"
#include "base/compiler_specific.h"

namespace aura {
class Window;
}

namespace ash {

class ASH_EXPORT WindowSelectorDelegate {
 public:
  
  virtual void OnWindowSelected(aura::Window* window) = 0;

  
  virtual void OnSelectionCanceled() = 0;

 protected:
  virtual ~WindowSelectorDelegate() {}
};

}  

#endif  
