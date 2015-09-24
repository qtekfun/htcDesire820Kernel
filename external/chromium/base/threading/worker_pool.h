// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_WORKER_POOL_H_
#define BASE_THREADING_WORKER_POOL_H_
#pragma once

#include "base/base_api.h"
#include "base/tracked.h"

class Task;

namespace base {

class BASE_API WorkerPool {
 public:
  
  
  
  
  static bool PostTask(const tracked_objects::Location& from_here,
                       Task* task, bool task_is_slow);
};

}  

#endif  
