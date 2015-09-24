// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_LOCK_STATE_CONTROLLER_H_
#define ASH_WM_LOCK_STATE_CONTROLLER_H_

#include "ash/ash_export.h"
#include "ash/shell_observer.h"
#include "ash/wm/lock_state_observer.h"
#include "ash/wm/session_state_animator.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "ui/aura/root_window_observer.h"

namespace gfx {
class Rect;
class Size;
}

namespace ui {
class Layer;
}

namespace ash {

namespace test {
class LockStateControllerTest;
class PowerButtonControllerTest;
}

class ASH_EXPORT LockStateControllerDelegate {
 public:
  LockStateControllerDelegate() {}
  virtual ~LockStateControllerDelegate() {}

  virtual void RequestLockScreen() = 0;
  virtual void RequestShutdown() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(LockStateControllerDelegate);
};

class ASH_EXPORT LockStateController : public aura::RootWindowObserver,
                                       public ShellObserver {
 public:
  
  
  static const int kLockTimeoutMs;

  
  static const int kShutdownTimeoutMs;

  
  
  static const int kLockFailTimeoutMs;

  
  
  
  static const int kLockToShutdownTimeoutMs;

  
  
  
  static const int kShutdownRequestDelayMs;

  
  class ASH_EXPORT TestApi {
   public:
    explicit TestApi(LockStateController* controller);

    virtual ~TestApi();

    bool lock_fail_timer_is_running() const {
      return controller_->lock_fail_timer_.IsRunning();
    }
    bool lock_to_shutdown_timer_is_running() const {
      return controller_->lock_to_shutdown_timer_.IsRunning();
    }
    bool shutdown_timer_is_running() const {
      return controller_->pre_shutdown_timer_.IsRunning();
    }
    bool real_shutdown_timer_is_running() const {
      return controller_->real_shutdown_timer_.IsRunning();
    }
    bool is_animating_lock() const {
      return controller_->animating_lock_;
    }
    bool is_lock_cancellable() const {
      return controller_->CanCancelLockAnimation();
    }

    void trigger_lock_fail_timeout() {
      controller_->OnLockFailTimeout();
      controller_->lock_fail_timer_.Stop();
    }
    void trigger_lock_to_shutdown_timeout() {
      controller_->OnLockToShutdownTimeout();
      controller_->lock_to_shutdown_timer_.Stop();
    }
    void trigger_shutdown_timeout() {
      controller_->OnPreShutdownAnimationTimeout();
      controller_->pre_shutdown_timer_.Stop();
    }
    void trigger_real_shutdown_timeout() {
      controller_->OnRealShutdownTimeout();
      controller_->real_shutdown_timer_.Stop();
    }
   private:
    LockStateController* controller_;  

    DISALLOW_COPY_AND_ASSIGN(TestApi);
  };

  LockStateController();
  virtual ~LockStateController();

  
  void SetDelegate(LockStateControllerDelegate* delegate);

  void AddObserver(LockStateObserver* observer);
  void RemoveObserver(LockStateObserver* observer);
  bool HasObserver(LockStateObserver* observer);

  
  
  
  
  void StartLockAnimation(bool shutdown_after_lock);

  
  void StartShutdownAnimation();

  
  
  void StartLockAnimationAndLockImmediately();

  
  
  bool LockRequested();

  
  bool ShutdownRequested();

  
  bool CanCancelLockAnimation();

  
  void CancelLockAnimation();

  
  bool CanCancelShutdownAnimation();

  
  void CancelShutdownAnimation();

  
  void OnStartingLock();

  
  void RequestShutdown();

  
  
  
  void OnLockScreenHide(base::Closure& callback);

  
  
  void SetLockScreenDisplayedCallback(const base::Closure& callback);

  
  virtual void OnRootWindowHostCloseRequested(
     const aura::RootWindow* root) OVERRIDE;

  
  virtual void OnLoginStateChanged(user::LoginStatus status) OVERRIDE;
  virtual void OnAppTerminating() OVERRIDE;
  virtual void OnLockStateChanged(bool locked) OVERRIDE;

 private:
  friend class test::PowerButtonControllerTest;
  friend class test::LockStateControllerTest;

  struct UnlockedStateProperties {
    bool background_is_hidden;
  };

  
  void OnLockFailTimeout();

  
  void StartLockToShutdownTimer();

  
  void OnLockToShutdownTimeout();

  
  void StartPreShutdownAnimationTimer();

  
  void OnPreShutdownAnimationTimeout();

  
  
  
  void StartRealShutdownTimer(bool with_animation_time);

  
  void OnRealShutdownTimeout();

  
  
  void StartCancellableShutdownAnimation();

  
  
  
  
  void StartImmediatePreLockAnimation(bool request_lock_on_completion);
  void StartCancellablePreLockAnimation();
  void CancelPreLockAnimation();
  void StartPostLockAnimation();
  
  void StartUnlockAnimationBeforeUIDestroyed(base::Closure &callback);
  void StartUnlockAnimationAfterUIDestroyed();

  
  void LockAnimationCancelled();
  void PreLockAnimationFinished(bool request_lock);
  void PostLockAnimationFinished();
  void UnlockAnimationAfterUIDestroyedFinished();

  
  void StoreUnlockedProperties();
  void RestoreUnlockedProperties();

  
  void AnimateBackgroundAppearanceIfNecessary(
      ash::internal::SessionStateAnimator::AnimationSpeed speed,
      ui::LayerAnimationObserver* observer);

  
  void AnimateBackgroundHidingIfNecessary(
      ash::internal::SessionStateAnimator::AnimationSpeed speed,
      ui::LayerAnimationObserver* observer);

  scoped_ptr<internal::SessionStateAnimator> animator_;

  scoped_ptr<LockStateControllerDelegate> delegate_;

  ObserverList<LockStateObserver> observers_;

  
  user::LoginStatus login_status_;

  
  bool system_is_locked_;

  
  bool shutting_down_;

  
  
  bool shutdown_after_lock_;

  
  bool animating_lock_;

  
  bool can_cancel_lock_animation_;

  scoped_ptr<UnlockedStateProperties> unlocked_properties_;

  
  
  base::OneShotTimer<LockStateController> lock_fail_timer_;

  
  
  
  base::OneShotTimer<LockStateController> lock_to_shutdown_timer_;

  
  
  base::OneShotTimer<LockStateController> pre_shutdown_timer_;

  
  
  
  base::OneShotTimer<LockStateController> real_shutdown_timer_;

  base::Closure lock_screen_displayed_callback_;

  DISALLOW_COPY_AND_ASSIGN(LockStateController);
};

}  

#endif  
