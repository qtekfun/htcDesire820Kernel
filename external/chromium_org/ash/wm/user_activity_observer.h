// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_WM_USER_ACTIVITY_OBSERVER_H_
#define ASH_WM_USER_ACTIVITY_OBSERVER_H_

#include "ash/ash_export.h"
#include "base/basictypes.h"

namespace ui {
class Event;
}

namespace ash {

class ASH_EXPORT UserActivityObserver {
 public:
  
  
  
  virtual void OnUserActivity(const ui::Event* event) = 0;

 protected:
  UserActivityObserver() {}
  virtual ~UserActivityObserver() {}

  DISALLOW_COPY_AND_ASSIGN(UserActivityObserver);
};

}  

#endif  
