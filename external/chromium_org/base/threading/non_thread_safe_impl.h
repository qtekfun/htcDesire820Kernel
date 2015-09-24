// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_NON_THREAD_SAFE_IMPL_H_
#define BASE_THREADING_NON_THREAD_SAFE_IMPL_H_

#include "base/base_export.h"
#include "base/threading/thread_checker_impl.h"

namespace base {

class BASE_EXPORT NonThreadSafeImpl {
 public:
  bool CalledOnValidThread() const;

 protected:
  ~NonThreadSafeImpl();

  
  
  
  
  
  void DetachFromThread();

 private:
  ThreadCheckerImpl thread_checker_;
};

}  

#endif  
