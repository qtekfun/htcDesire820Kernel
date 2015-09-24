// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RLZ_LIB_RECURSIVE_CROSS_PROCESS_LOCK_POSIX_H_
#define RLZ_LIB_RECURSIVE_CROSS_PROCESS_LOCK_POSIX_H_

#include <pthread.h>

namespace base {
class FilePath;
}

namespace rlz_lib {

struct RecursiveCrossProcessLock {
  
  
  
  bool TryGetCrossProcessLock(const base::FilePath& lock_filename);

  
  
  void ReleaseLock();

  pthread_mutex_t recursive_lock_;
  pthread_t locking_thread_;

  int file_lock_;
};

#define RECURSIVE_CROSS_PROCESS_LOCK_INITIALIZER    \
  { PTHREAD_MUTEX_INITIALIZER, 0, -1 }

}  

#endif  
