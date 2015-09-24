// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_ONE_SHOT_EVENT_H_
#define EXTENSIONS_COMMON_ONE_SHOT_EVENT_H_

#include <vector>

#include "base/callback_forward.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"

namespace base {
class TaskRunner;
}

namespace tracked_objects {
class Location;
}

namespace extensions {

class OneShotEvent {
 public:
  OneShotEvent();
  ~OneShotEvent();

  
  
  
  bool is_signaled() const {
    DCHECK(thread_checker_.CalledOnValidThread());
    return signaled_;
  }

  
  
  void Signal();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void Post(const tracked_objects::Location& from_here,
            const base::Closure& task) const;
  void Post(const tracked_objects::Location& from_here,
            const base::Closure& task,
            const scoped_refptr<base::TaskRunner>& runner) const;

 private:
  struct TaskInfo;

  base::ThreadChecker thread_checker_;

  bool signaled_;

  
  
  
  
  
  
  
  
  
  mutable std::vector<TaskInfo> tasks_;
};

}  

#endif  
