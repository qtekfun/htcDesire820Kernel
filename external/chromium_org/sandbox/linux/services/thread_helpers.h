// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_LINUX_SERVICES_THREAD_HELPERS_H_
#define SANDBOX_LINUX_SERVICES_THREAD_HELPERS_H_

#include "base/basictypes.h"

namespace base { class Thread; }

namespace sandbox {

class ThreadHelpers {
 public:
  
  
  
  static bool IsSingleThreaded(int proc_self_task);

  
  
  
  static bool StopThreadAndWatchProcFS(int proc_self_tasks,
                                       base::Thread* thread);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(ThreadHelpers);
};

}  

#endif  
