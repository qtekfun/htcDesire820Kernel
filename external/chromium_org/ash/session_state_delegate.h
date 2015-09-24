// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SESSION_STATE_DELEGATE_H_
#define ASH_SESSION_STATE_DELEGATE_H_

#include <string>
#include <vector>

#include "ash/ash_export.h"
#include "base/strings/string16.h"

namespace aura {
class Window;
}  

namespace gfx {
class ImageSkia;
}  

namespace ash {

class SessionStateObserver;

typedef int MultiProfileIndex;

typedef std::vector<std::string> UserIdList;

class ASH_EXPORT SessionStateDelegate {
 public:
  
  enum CycleUser {
    CYCLE_TO_NEXT_USER = 0,  
    CYCLE_TO_PREVIOUS_USER,  
  };

  virtual ~SessionStateDelegate() {};

  
  virtual int GetMaximumNumberOfLoggedInUsers() const = 0;

  
  
  virtual int NumberOfLoggedInUsers() const = 0;

  
  
  
  
  virtual bool IsActiveUserSessionStarted() const = 0;

  
  virtual bool CanLockScreen() const = 0;

  
  virtual bool IsScreenLocked() const = 0;

  
  
  virtual bool ShouldLockScreenBeforeSuspending() const = 0;

  
  virtual void LockScreen() = 0;

  
  virtual void UnlockScreen() = 0;

  
  
  
  virtual bool IsUserSessionBlocked() const = 0;

  
  
  virtual const base::string16 GetUserDisplayName(
      MultiProfileIndex index) const = 0;

  
  
  
  
  virtual const std::string GetUserEmail(MultiProfileIndex index) const = 0;

  
  
  
  virtual const std::string GetUserID(MultiProfileIndex index) const = 0;

  
  
  virtual const gfx::ImageSkia& GetUserImage(MultiProfileIndex index) const = 0;

  
  virtual void GetLoggedInUsers(UserIdList* users) = 0;

  
  
  virtual void SwitchActiveUser(const std::string& user_id) = 0;

  
  
  virtual void CycleActiveUser(CycleUser cycle_user) = 0;

  
  virtual void AddSessionStateObserver(SessionStateObserver* observer) = 0;
  virtual void RemoveSessionStateObserver(SessionStateObserver* observer) = 0;

  
  
  
  virtual bool TransferWindowToDesktopOfUser(
      aura::Window* window,
      ash::MultiProfileIndex index) = 0;
};

}  

#endif  
