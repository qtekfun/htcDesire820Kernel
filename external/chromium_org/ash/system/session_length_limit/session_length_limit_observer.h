// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_SESSION_LENGTH_LIMIT_SESSION_LENGTH_LIMIT_OBSERVER_H_
#define ASH_SYSTEM_SESSION_LENGTH_LIMIT_SESSION_LENGTH_LIMIT_OBSERVER_H_

#include "ash/ash_export.h"

namespace ash {

class ASH_EXPORT SessionLengthLimitObserver {
 public:
  virtual ~SessionLengthLimitObserver() {}

  
  virtual void OnSessionStartTimeChanged() = 0;

  
  virtual void OnSessionLengthLimitChanged() = 0;
};

}  

#endif  
