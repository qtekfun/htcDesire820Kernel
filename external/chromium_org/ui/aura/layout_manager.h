// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_LAYOUT_MANAGER_H_
#define UI_AURA_LAYOUT_MANAGER_H_

#include "base/basictypes.h"
#include "ui/aura/aura_export.h"

namespace gfx {
class Rect;
}

namespace aura {
class Window;

class AURA_EXPORT LayoutManager {
 public:
  LayoutManager();
  virtual ~LayoutManager();

  
  virtual void OnWindowResized() = 0;

  
  virtual void OnWindowAddedToLayout(Window* child) = 0;

  
  virtual void OnWillRemoveWindowFromLayout(Window* child) = 0;

  
  virtual void OnWindowRemovedFromLayout(Window* child) = 0;

  
  
  
  
  virtual void OnChildWindowVisibilityChanged(Window* child, bool visible) = 0;

  
  
  
  
  virtual void SetChildBounds(Window* child,
                              const gfx::Rect& requested_bounds) = 0;

 protected:
  
  
  
  void SetChildBoundsDirect(aura::Window* child, const gfx::Rect& bounds);
};

}  

#endif  
