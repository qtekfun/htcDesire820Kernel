// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_TEST_TEST_SESSION_STATE_DELEGATE_H_
#define ASH_TEST_TEST_SESSION_STATE_DELEGATE_H_

#include "ash/session_state_delegate.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ui/gfx/image/image_skia.h"

namespace ash {
namespace test {

class TestSessionStateDelegate : public SessionStateDelegate {
 public:
  TestSessionStateDelegate();
  virtual ~TestSessionStateDelegate();

  void set_logged_in_users(int users) { logged_in_users_ = users; }
  const std::string& get_activated_user() { return activated_user_; }

  
  virtual int GetMaximumNumberOfLoggedInUsers() const OVERRIDE;
  virtual int NumberOfLoggedInUsers() const OVERRIDE;
  virtual bool IsActiveUserSessionStarted() const OVERRIDE;
  virtual bool CanLockScreen() const OVERRIDE;
  virtual bool IsScreenLocked() const OVERRIDE;
  virtual bool ShouldLockScreenBeforeSuspending() const OVERRIDE;
  virtual void LockScreen() OVERRIDE;
  virtual void UnlockScreen() OVERRIDE;
  virtual bool IsUserSessionBlocked() const OVERRIDE;
  virtual const base::string16 GetUserDisplayName(
      ash::MultiProfileIndex index) const OVERRIDE;
  virtual const std::string GetUserEmail(
      ash::MultiProfileIndex index) const OVERRIDE;
  virtual const std::string GetUserID(
      ash::MultiProfileIndex index) const OVERRIDE;
  virtual const gfx::ImageSkia& GetUserImage(
      ash::MultiProfileIndex index) const OVERRIDE;
  virtual void GetLoggedInUsers(UserIdList* users) OVERRIDE;
  virtual void SwitchActiveUser(const std::string& user_id) OVERRIDE;
  virtual void CycleActiveUser(CycleUser cycle_user) OVERRIDE;
  virtual void AddSessionStateObserver(
      ash::SessionStateObserver* observer) OVERRIDE;
  virtual void RemoveSessionStateObserver(
      ash::SessionStateObserver* observer) OVERRIDE;
  virtual bool TransferWindowToDesktopOfUser(
      aura::Window* window,
      ash::MultiProfileIndex index) OVERRIDE;

  

  
  
  
  
  void SetHasActiveUser(bool has_active_user);

  
  
  
  
  void SetActiveUserSessionStarted(bool active_user_session_started);

  
  
  
  void SetCanLockScreen(bool can_lock_screen);

  
  void SetShouldLockScreenBeforeSuspending(bool should_lock);

  
  
  void SetUserAddingScreenRunning(bool user_adding_screen_running);

  
  int num_transfer_to_desktop_of_user_calls() {
    return num_transfer_to_desktop_of_user_calls_;
  }

 private:
  
  bool has_active_user_;

  
  
  
  bool active_user_session_started_;

  
  
  bool can_lock_screen_;

  
  bool should_lock_screen_before_suspending_;

  
  bool screen_locked_;

  
  bool user_adding_screen_running_;

  
  int logged_in_users_;

  
  std::string activated_user_;

  
  gfx::ImageSkia null_image_;

  
  int num_transfer_to_desktop_of_user_calls_;

  DISALLOW_COPY_AND_ASSIGN(TestSessionStateDelegate);
};

}  
}  

#endif  
