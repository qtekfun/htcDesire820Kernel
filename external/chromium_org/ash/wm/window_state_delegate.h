// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_WINDOW_STATE_DELEGATE_H_
#define ASH_WM_WINDOW_STATE_DELEGATE_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"

namespace ash {
namespace wm {
class WindowState;

class ASH_EXPORT WindowStateDelegate {
 public:
  WindowStateDelegate();
  virtual ~WindowStateDelegate();

  
  
  
  
  
  
  virtual bool ToggleFullscreen(WindowState* window_state);

 private:
  DISALLOW_COPY_AND_ASSIGN(WindowStateDelegate);
};

}  
}  

#endif  
