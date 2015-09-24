// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_LOCK_STATE_OBSERVER_H_
#define ASH_WM_LOCK_STATE_OBSERVER_H_

#include "ash/ash_export.h"

namespace ash {

class ASH_EXPORT LockStateObserver {
 public:
  enum EventType {
    EVENT_PRELOCK_ANIMATION_STARTED,
    EVENT_LOCK_ANIMATION_STARTED,
    EVENT_LOCK_ANIMATION_FINISHED,
  };

  virtual void OnLockStateEvent(EventType event) = 0;
  virtual ~LockStateObserver() {}
};

}  

#endif  
