// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_SESSION_STATE_ANIMATOR_H_
#define ASH_WM_SESSION_STATE_ANIMATOR_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/timer/timer.h"
#include "ui/aura/window.h"
#include "ui/compositor/layer_animation_observer.h"

namespace gfx {
class Rect;
class Size;
}

namespace ui {
class Layer;
}

namespace ash {
namespace internal {

class ASH_EXPORT SessionStateAnimator {
 public:
  
  enum AnimationType {
    ANIMATION_PARTIAL_CLOSE = 0,
    ANIMATION_UNDO_PARTIAL_CLOSE,
    ANIMATION_FULL_CLOSE,
    ANIMATION_FADE_IN,
    ANIMATION_FADE_OUT,
    ANIMATION_HIDE_IMMEDIATELY,
    ANIMATION_RESTORE,
    
    
    ANIMATION_LIFT,
    ANIMATION_UNDO_LIFT,
    ANIMATION_DROP,
    
    ANIMATION_RAISE_TO_SCREEN,
    ANIMATION_LOWER_BELOW_SCREEN,
    ANIMATION_PARTIAL_FADE_IN,
    ANIMATION_UNDO_PARTIAL_FADE_IN,
    ANIMATION_FULL_FADE_IN,
    ANIMATION_GRAYSCALE_BRIGHTNESS,
    ANIMATION_UNDO_GRAYSCALE_BRIGHTNESS,
  };

  
  enum AnimationSpeed {
    
    ANIMATION_SPEED_IMMEDIATE = 0,
    
    
    ANIMATION_SPEED_UNDOABLE,
    
    
    ANIMATION_SPEED_REVERT,
    
    
    
    ANIMATION_SPEED_FAST,
    
    ANIMATION_SPEED_SHOW_LOCK_SCREEN,
    
    ANIMATION_SPEED_MOVE_WINDOWS,
    
    ANIMATION_SPEED_UNDO_MOVE_WINDOWS,
    
    ANIMATION_SPEED_SHUTDOWN,
    
    ANIMATION_SPEED_REVERT_SHUTDOWN,
  };

  
  enum Container {
    DESKTOP_BACKGROUND = 1 << 0,
    LAUNCHER = 1 << 1,

    
    
    NON_LOCK_SCREEN_CONTAINERS = 1 << 2,

    
    
    
    
    LOCK_SCREEN_BACKGROUND = 1 << 3,

    
    LOCK_SCREEN_CONTAINERS = 1 << 4,

    
    
    LOCK_SCREEN_RELATED_CONTAINERS = 1 << 5,
  };

  
  class ASH_EXPORT TestApi {
   public:
    explicit TestApi(SessionStateAnimator* animator)
        : animator_(animator) {}

    
    
    
    bool ContainersAreAnimated(int container_mask, AnimationType type) const;

    
    
    bool RootWindowIsAnimated(AnimationType type) const;

   private:
    SessionStateAnimator* animator_;  

    DISALLOW_COPY_AND_ASSIGN(TestApi);
  };

  
  
  const static int kAllLockScreenContainersMask;

  
  const static int kAllContainersMask;

  SessionStateAnimator();
  virtual ~SessionStateAnimator();

  
  static base::TimeDelta GetDuration(AnimationSpeed speed);

  
  static void GetContainers(int container_mask,
                            aura::Window::Windows* containers);

  
  
  void StartAnimation(int container_mask,
                      AnimationType type,
                      AnimationSpeed speed);

  
  
  
  void StartAnimationWithCallback(int container_mask,
                                  AnimationType type,
                                  AnimationSpeed speed,
                                  base::Callback<void(void)>& callback);

  void StartAnimationWithObserver(int container_mask,
                                  AnimationType type,
                                  AnimationSpeed speed,
                                  ui::LayerAnimationObserver* observer);

  
  void StartGlobalAnimation(AnimationType type,
                            AnimationSpeed speed);

  
  
  void RunAnimationForWindow(aura::Window* window,
                             AnimationType type,
                             AnimationSpeed speed,
                             ui::LayerAnimationObserver* observer);

  DISALLOW_COPY_AND_ASSIGN(SessionStateAnimator);
};

}  
}  

#endif  
