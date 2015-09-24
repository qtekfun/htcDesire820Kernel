// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_WINDOW_OBSERVER_H_
#define UI_AURA_WINDOW_OBSERVER_H_

#include "base/basictypes.h"
#include "ui/aura/aura_export.h"

namespace gfx {
class Rect;
}  

namespace aura {

class Window;

class AURA_EXPORT WindowObserver {
 public:
  struct HierarchyChangeParams {
    enum HierarchyChangePhase {
      HIERARCHY_CHANGING,
      HIERARCHY_CHANGED
    };

    Window* target;     
    Window* new_parent;
    Window* old_parent;
    HierarchyChangePhase phase;
    Window* receiver;   
  };

  
  
  
  
  
  
  
  
  
  
  virtual void OnWindowHierarchyChanging(const HierarchyChangeParams& params) {}
  virtual void OnWindowHierarchyChanged(const HierarchyChangeParams& params) {}

  
  virtual void OnWindowAdded(Window* new_window) {}

  
  virtual void OnWillRemoveWindow(Window* window) {}

  
  virtual void OnWindowParentChanged(Window* window, Window* parent) {}

  
  
  
  
  
  
  virtual void OnWindowPropertyChanged(Window* window,
                                       const void* key,
                                       intptr_t old) {}

  
  
  
  virtual void OnWindowVisibilityChanging(Window* window, bool visible) {}
  virtual void OnWindowVisibilityChanged(Window* window, bool visible) {}

  
  
  virtual void OnWindowBoundsChanged(Window* window,
                                     const gfx::Rect& old_bounds,
                                     const gfx::Rect& new_bounds) {}

  
  
  virtual void OnWindowStackingChanged(Window* window) {}

  
  virtual void OnWindowPaintScheduled(Window* window,
                                      const gfx::Rect& region) {}

  
  
  virtual void OnWindowDestroying(Window* window) {}

  
  
  
  
  
  
  
  
  virtual void OnWindowDestroyed(Window* window) {}

  
  virtual void OnWindowAddedToRootWindow(Window* window) {}

  
  virtual void OnWindowRemovingFromRootWindow(Window* window) {}

  
  virtual void OnAddTransientChild(Window* window, Window* transient) {}

  
  virtual void OnRemoveTransientChild(Window* window, Window* transient) {}

 protected:
  virtual ~WindowObserver() {}
};

}  

#endif  
