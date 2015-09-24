// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_POWER_USER_ACTIVITY_NOTIFIER_H_
#define ASH_SYSTEM_CHROMEOS_POWER_USER_ACTIVITY_NOTIFIER_H_

#include "ash/wm/user_activity_observer.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/time/time.h"

namespace ash {

class UserActivityDetector;

namespace internal {

class UserActivityNotifier : public UserActivityObserver {
 public:
  explicit UserActivityNotifier(UserActivityDetector* detector);
  virtual ~UserActivityNotifier();

  
  virtual void OnUserActivity(const ui::Event* event) OVERRIDE;

 private:
  UserActivityDetector* detector_;  

  
  base::TimeTicks last_notify_time_;

  DISALLOW_COPY_AND_ASSIGN(UserActivityNotifier);
};

}  
}  

#endif  
