// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_NON_THREAD_SAFE_H_
#define BASE_THREADING_NON_THREAD_SAFE_H_

#include "base/compiler_specific.h"

#if (!defined(NDEBUG) || defined(DCHECK_ALWAYS_ON))
#define ENABLE_NON_THREAD_SAFE 1
#else
#define ENABLE_NON_THREAD_SAFE 0
#endif

#if ENABLE_NON_THREAD_SAFE
#include "base/threading/non_thread_safe_impl.h"
#endif

namespace base {

class NonThreadSafeDoNothing {
 public:
  bool CalledOnValidThread() const {
    return true;
  }

 protected:
  ~NonThreadSafeDoNothing() {}
  void DetachFromThread() {}
};

#if ENABLE_NON_THREAD_SAFE
typedef NonThreadSafeImpl NonThreadSafe;
#else
typedef NonThreadSafeDoNothing NonThreadSafe;
#endif  

#undef ENABLE_NON_THREAD_SAFE

}  

#endif  
