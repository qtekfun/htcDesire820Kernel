// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_STARTUP_TASK_RUNNER_H_
#define CONTENT_BROWSER_STARTUP_TASK_RUNNER_H_

#include <list>

#include "base/callback.h"
#include "base/single_thread_task_runner.h"

#include "build/build_config.h"

#include "content/public/browser/browser_main_runner.h"

namespace content {

typedef base::Callback<int(void)> StartupTask;


class CONTENT_EXPORT StartupTaskRunner {

 public:
  
  
  StartupTaskRunner(base::Callback<void(int)> startup_complete_callback,
                    scoped_refptr<base::SingleThreadTaskRunner> proxy);

  ~StartupTaskRunner();

  
  void AddTask(StartupTask& callback);

  
  void StartRunningTasksAsync();

  
  void RunAllTasksNow();

 private:
  friend class base::RefCounted<StartupTaskRunner>;

  std::list<StartupTask> task_list_;
  void WrappedTask();

  base::Callback<void(int)> startup_complete_callback_;
  scoped_refptr<base::SingleThreadTaskRunner> proxy_;

  DISALLOW_COPY_AND_ASSIGN(StartupTaskRunner);
};

}  
#endif  
