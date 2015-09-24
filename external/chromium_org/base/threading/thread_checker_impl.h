// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_THREAD_CHECKER_IMPL_H_
#define BASE_THREADING_THREAD_CHECKER_IMPL_H_

#include "base/base_export.h"
#include "base/synchronization/lock.h"
#include "base/threading/platform_thread.h"

namespace base {

class BASE_EXPORT ThreadCheckerImpl {
 public:
  ThreadCheckerImpl();
  ~ThreadCheckerImpl();

  bool CalledOnValidThread() const;

  
  
  
  void DetachFromThread();

 private:
  void EnsureThreadIdAssigned() const;

  mutable base::Lock lock_;
  
  
  mutable PlatformThreadId valid_thread_id_;
};

}  

#endif  