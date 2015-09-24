// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_SYNCHRONIZATION_CANCELLATION_FLAG_H_
#define BASE_SYNCHRONIZATION_CANCELLATION_FLAG_H_

#include "base/base_export.h"
#include "base/atomicops.h"
#include "base/threading/platform_thread.h"

namespace base {

class BASE_EXPORT CancellationFlag {
 public:
  CancellationFlag() : flag_(false) {
#if !defined(NDEBUG)
    set_on_ = PlatformThread::CurrentId();
#endif
  }
  ~CancellationFlag() {}

  
  void Set();
  bool IsSet() const;  

 private:
  base::subtle::Atomic32 flag_;
#if !defined(NDEBUG)
  PlatformThreadId set_on_;
#endif

  DISALLOW_COPY_AND_ASSIGN(CancellationFlag);
};

}  

#endif  
