// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_NON_THREAD_SAFE_H_
#define BASE_THREADING_NON_THREAD_SAFE_H_
#pragma once

#ifndef NDEBUG
#include "base/threading/non_thread_safe_impl.h"
#endif

namespace base {

class NonThreadSafeDoNothing {
 public:
  bool CalledOnValidThread() const {
    return true;
  }

 protected:
  void DetachFromThread() {}
};

#ifndef NDEBUG
class NonThreadSafe : public NonThreadSafeImpl {
};
#else
class NonThreadSafe : public NonThreadSafeDoNothing {
};
#endif  

}  

#endif  
