// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_IDLE_H_
#define CHROME_BROWSER_IDLE_H_

#include "base/callback.h"

enum IdleState {
  IDLE_STATE_ACTIVE = 0,
  IDLE_STATE_IDLE = 1,    
  IDLE_STATE_LOCKED = 2,  
  IDLE_STATE_UNKNOWN = 3  
                          
};

#if defined(OS_MACOSX)
void InitIdleMonitor();
#endif

typedef base::Callback<void(IdleState)> IdleCallback;
typedef base::Callback<void(int)> IdleTimeCallback;

void CalculateIdleState(int idle_threshold, IdleCallback notify);

void CalculateIdleTime(IdleTimeCallback notify);

bool CheckIdleStateIsLocked();

#endif  
