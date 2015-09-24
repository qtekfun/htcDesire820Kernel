// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SHELF_SHELF_LAYOUT_MANAGER_OBSERVER_H_
#define ASH_SHELF_SHELF_LAYOUT_MANAGER_OBSERVER_H_

#include "ash/ash_export.h"
#include "ash/shelf/background_animator.h"
#include "ash/shelf/shelf_types.h"

namespace aura {
class Window;
}

namespace ash {

class ASH_EXPORT ShelfLayoutManagerObserver {
 public:
  virtual ~ShelfLayoutManagerObserver() {}

  
  virtual void WillDeleteShelf() {}

  
  virtual void WillChangeVisibilityState(ShelfVisibilityState new_state) {}

  
  virtual void OnAutoHideStateChanged(ShelfAutoHideState new_state) {}

  
  virtual void OnAutoHideBehaviorChanged(aura::Window* root_window,
                                         ShelfAutoHideBehavior new_behavior) {}

  
  virtual void OnBackgroundUpdated(
      ShelfBackgroundType background_type,
      BackgroundAnimatorChangeType change_type) {}
};

}  

#endif  
