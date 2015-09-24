// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_SESSION_LENGTH_LIMITER_H_
#define CHROME_BROWSER_CHROMEOS_SESSION_LENGTH_LIMITER_H_

#include "ash/wm/user_activity_observer.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/threading/thread_checker.h"
#include "base/time/time.h"
#include "base/timer/timer.h"

class PrefService;
class PrefRegistrySimple;

namespace chromeos {

class SessionLengthLimiter : public ash::UserActivityObserver {
 public:
  class Delegate {
   public:
    virtual ~Delegate();

    virtual const base::TimeTicks GetCurrentTime() const = 0;
    virtual void StopSession() = 0;
  };

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

  SessionLengthLimiter(Delegate* delegate, bool browser_restarted);
  virtual ~SessionLengthLimiter();

  
  virtual void OnUserActivity(const ui::Event* event) OVERRIDE;

 private:
  
  
  bool RestoreStateAfterCrash();

  
  
  
  
  
  
  
  
  
  void UpdateSessionStartTime();

  void UpdateLimit();

  base::ThreadChecker thread_checker_;

  scoped_ptr<Delegate> delegate_;
  PrefChangeRegistrar pref_change_registrar_;

  scoped_ptr<base::OneShotTimer<SessionLengthLimiter::Delegate> > timer_;
  base::TimeTicks session_start_time_;
  bool user_activity_seen_;

  DISALLOW_COPY_AND_ASSIGN(SessionLengthLimiter);
};

}  

#endif  
