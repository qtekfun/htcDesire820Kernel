// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef ASH_SCOPED_TARGET_ROOT_WINDOW_H_
#define ASH_SCOPED_TARGET_ROOT_WINDOW_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"

namespace aura {
class Window;
}

namespace ash {
namespace internal {

class ASH_EXPORT ScopedTargetRootWindow {
 public:
  explicit ScopedTargetRootWindow(aura::Window* root_window);
  ~ScopedTargetRootWindow();

 private:
  DISALLOW_COPY_AND_ASSIGN(ScopedTargetRootWindow);
};

}  
}  

#endif  
