// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_ROOT_WINDOW_OBSERVER_H_
#define UI_AURA_ROOT_WINDOW_OBSERVER_H_

#include "ui/aura/aura_export.h"

namespace gfx {
class Point;
class Size;
}

namespace aura {
class RootWindow;
class Window;

class AURA_EXPORT RootWindowObserver {
 public:
  
  virtual void OnRootWindowHostResized(const RootWindow* root) {}

  
  virtual void OnRootWindowHostMoved(const RootWindow* root,
                                     const gfx::Point& new_origin) {}

  
  
  virtual void OnRootWindowHostCloseRequested(const RootWindow* root) {}

  
  virtual void OnKeyboardMappingChanged(const RootWindow* root) {}

 protected:
  virtual ~RootWindowObserver() {}
};

}  

#endif  
