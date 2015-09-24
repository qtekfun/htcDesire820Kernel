// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_CROS_SCREEN_LOCK_LIBRARY_H_
#define CHROME_BROWSER_CHROMEOS_CROS_SCREEN_LOCK_LIBRARY_H_
#pragma once

#include "base/observer_list.h"
#include "third_party/cros/chromeos_screen_lock.h"

namespace chromeos {

class ScreenLockLibrary {
 public:
  class Observer {
   public:
    virtual ~Observer() {}
    virtual void LockScreen(ScreenLockLibrary* obj) = 0;
    virtual void UnlockScreen(ScreenLockLibrary* obj) = 0;
    virtual void UnlockScreenFailed(ScreenLockLibrary* obj) = 0;
  };
  ScreenLockLibrary() {}
  virtual ~ScreenLockLibrary() {}
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  
  virtual void NotifyScreenLockCompleted() = 0;
  
  virtual void NotifyScreenLockRequested() = 0;
  
  virtual void NotifyScreenUnlockRequested() = 0;
  
  virtual void NotifyScreenUnlockCompleted() = 0;

  
  
  static ScreenLockLibrary* GetImpl(bool stub);
};

}  

#endif  
