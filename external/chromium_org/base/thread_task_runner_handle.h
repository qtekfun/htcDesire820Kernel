// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_THREAD_TASK_RUNNER_HANDLE_H_
#define BASE_THREAD_TASK_RUNNER_HANDLE_H_

#include "base/base_export.h"
#include "base/memory/ref_counted.h"

namespace base {

class SingleThreadTaskRunner;

class BASE_EXPORT ThreadTaskRunnerHandle {
 public:
  
  static scoped_refptr<SingleThreadTaskRunner> Get();

  
  
  static bool IsSet();

  
  
  explicit ThreadTaskRunnerHandle(
      const scoped_refptr<SingleThreadTaskRunner>& task_runner);
  ~ThreadTaskRunnerHandle();

 private:
  scoped_refptr<SingleThreadTaskRunner> task_runner_;
};

}  

#endif  
