// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_SYSTEM_WAITER_H_
#define MOJO_SYSTEM_WAITER_H_

#include "base/basictypes.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "mojo/public/system/core.h"
#include "mojo/system/system_impl_export.h"

namespace mojo {
namespace system {

class MOJO_SYSTEM_IMPL_EXPORT Waiter {
 public:
  Waiter();
  ~Waiter();

  
  
  void Init();

  
  
  
  
  
  
  
  
  
  
  MojoResult Wait(MojoDeadline deadline);

  
  
  void Awake(MojoResult wait_result);

 private:
  base::ConditionVariable cv_;  
  base::Lock lock_;  
#ifndef NDEBUG
  bool initialized_;
#endif
  bool awoken_;
  MojoResult wait_result_;

  DISALLOW_COPY_AND_ASSIGN(Waiter);
};

}  
}  

#endif  
