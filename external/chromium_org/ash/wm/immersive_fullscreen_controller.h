// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_IMMERSIVE_FULLSCREEN_CONTROLLER_H_
#define ASH_WM_IMMERSIVE_FULLSCREEN_CONTROLLER_H_

#include <vector>

#include "ash/ash_export.h"
#include "ash/wm/immersive_revealed_lock.h"
#include "base/timer/timer.h"
#include "ui/aura/window_observer.h"
#include "ui/events/event_handler.h"
#include "ui/gfx/animation/animation_delegate.h"
#include "ui/views/focus/focus_manager.h"
#include "ui/views/widget/widget_observer.h"

namespace aura {
class Window;
}

namespace gfx {
class Point;
class Rect;
class SlideAnimation;
}

namespace ui {
class LocatedEvent;
}

namespace views {
class View;
class Widget;
}

namespace ash {

class ASH_EXPORT ImmersiveFullscreenController
    : public gfx::AnimationDelegate,
      public ui::EventHandler,
      public views::FocusChangeListener,
      public views::WidgetObserver,
      public aura::WindowObserver,
      public ImmersiveRevealedLock::Delegate {
 public:
  
  
  enum WindowType {
    WINDOW_TYPE_OTHER,
    WINDOW_TYPE_BROWSER,
    WINDOW_TYPE_HOSTED_APP,
    WINDOW_TYPE_PACKAGED_APP,
    WINDOW_TYPE_COUNT
  };

  class Delegate {
   public:
    
    virtual void OnImmersiveRevealStarted() = 0;

    
    
    
    virtual void OnImmersiveRevealEnded() = 0;

    
    virtual void OnImmersiveFullscreenExited() = 0;

    
    
    virtual void SetVisibleFraction(double visible_fraction) = 0;

    
    
    
    
    virtual std::vector<gfx::Rect> GetVisibleBoundsInScreen() const = 0;

   protected:
    virtual ~Delegate() {}
  };

  ImmersiveFullscreenController();
  virtual ~ImmersiveFullscreenController();

  
  
  
  
  
  void Init(Delegate* delegate,
            views::Widget* widget,
            views::View* top_container);

  
  
  
  void SetEnabled(WindowType window_type, bool enable);

  
  bool IsEnabled() const;

  
  
  bool IsRevealed() const;

  
  
  
  
  
  
  
  
  
  ImmersiveRevealedLock* GetRevealedLock(
      AnimateReveal animate_reveal) WARN_UNUSED_RESULT;

  
  
  void SetupForTest();

  
  virtual void OnMouseEvent(ui::MouseEvent* event) OVERRIDE;
  virtual void OnTouchEvent(ui::TouchEvent* event) OVERRIDE;
  virtual void OnGestureEvent(ui::GestureEvent* event) OVERRIDE;

  
  virtual void OnWillChangeFocus(views::View* focused_before,
                                 views::View* focused_now) OVERRIDE;
  virtual void OnDidChangeFocus(views::View* focused_before,
                                views::View* focused_now) OVERRIDE;

  
  virtual void OnWidgetDestroying(views::Widget* widget) OVERRIDE;
  virtual void OnWidgetActivationChanged(views::Widget* widget,
                                         bool active) OVERRIDE;

  
  virtual void AnimationEnded(const gfx::Animation* animation) OVERRIDE;
  virtual void AnimationProgressed(const gfx::Animation* animation) OVERRIDE;

  
  virtual void OnAddTransientChild(aura::Window* window,
                                   aura::Window* transient) OVERRIDE;
  virtual void OnRemoveTransientChild(aura::Window* window,
                                      aura::Window* transient) OVERRIDE;

  
  virtual void LockRevealedState(AnimateReveal animate_reveal) OVERRIDE;
  virtual void UnlockRevealedState() OVERRIDE;

 private:
  friend class ImmersiveFullscreenControllerTest;

  enum Animate {
    ANIMATE_NO,
    ANIMATE_SLOW,
    ANIMATE_FAST,
  };
  enum RevealState {
    CLOSED,
    SLIDING_OPEN,
    REVEALED,
    SLIDING_CLOSED,
  };
  enum SwipeType {
    SWIPE_OPEN,
    SWIPE_CLOSE,
    SWIPE_NONE
  };

  
  void EnableWindowObservers(bool enable);

  
  
  
  
  void UpdateTopEdgeHoverTimer(ui::MouseEvent* event);

  
  
  
  void UpdateLocatedEventRevealedLock(ui::LocatedEvent* event);

  
  void AcquireLocatedEventRevealedLock();

  
  
  void UpdateFocusRevealedLock();

  
  
  
  bool UpdateRevealedLocksForSwipe(SwipeType swipe_type);

  
  int GetAnimationDuration(Animate animate) const;

  
  
  
  void MaybeStartReveal(Animate animate);

  
  
  void OnSlideOpenAnimationCompleted();

  
  
  void MaybeEndReveal(Animate animate);

  
  
  void OnSlideClosedAnimationCompleted();

  
  SwipeType GetSwipeType(ui::GestureEvent* event) const;

  
  
  
  bool ShouldIgnoreMouseEventAtLocation(
      const gfx::Point& location_in_screen) const;

  
  
  
  
  
  
  bool ShouldHandleGestureEvent(const gfx::Point& location) const;

  
  
  void RecreateBubbleManager();

  
  Delegate* delegate_;
  views::View* top_container_;
  views::Widget* widget_;
  aura::Window* native_window_;

  
  bool observers_enabled_;

  
  bool enabled_;

  
  RevealState reveal_state_;

  int revealed_lock_count_;

  
  base::OneShotTimer<ImmersiveFullscreenController> top_edge_hover_timer_;

  
  
  int mouse_x_when_hit_top_in_screen_;

  
  
  bool gesture_begun_;

  
  
  
  
  scoped_ptr<ImmersiveRevealedLock> located_event_revealed_lock_;

  
  
  
  scoped_ptr<ImmersiveRevealedLock> focus_revealed_lock_;

  
  scoped_ptr<gfx::SlideAnimation> animation_;

  
  bool animations_disabled_for_test_;

  
  class BubbleManager;
  scoped_ptr<BubbleManager> bubble_manager_;

  base::WeakPtrFactory<ImmersiveFullscreenController> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ImmersiveFullscreenController);
};

}  

#endif  
