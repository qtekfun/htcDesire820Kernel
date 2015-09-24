// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREADING_NON_THREAD_SAFE_IMPL_H_
#define BASE_THREADING_NON_THREAD_SAFE_IMPL_H_
#pragma once

#include "base/base_api.h"
#include "base/threading/thread_checker_impl.h"

namespace base {

class BASE_API NonThreadSafeImpl {
 public:
  ~NonThreadSafeImpl();

  bool CalledOnValidThread() const;

 protected:
  
  
  
  
  
  void DetachFromThread();

 private:
  ThreadCheckerImpl thread_checker_;
};

}  

#endif  
