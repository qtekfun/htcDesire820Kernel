// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_TASK_MARSHALLER_H_
#define CHROME_FRAME_TASK_MARSHALLER_H_

#include <windows.h>
#include <deque>
#include <queue>

#include "base/callback.h"
#include "base/pending_task.h"
#include "base/synchronization/lock.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"

class Task;

namespace tracked_objects {
class Location;
}

class TaskMarshallerThroughMessageQueue : public base::NonThreadSafe {
 public:
  TaskMarshallerThroughMessageQueue();
  virtual ~TaskMarshallerThroughMessageQueue();

  void SetWindow(HWND wnd, UINT msg) {
    wnd_ = wnd;
    msg_ = msg;
  }

  virtual void PostTask(const tracked_objects::Location& from_here,
                        const base::Closure& task);
  virtual void PostDelayedTask(const tracked_objects::Location& source,
                               const base::Closure& task,
                               base::TimeDelta& delay);

  
  BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                            LRESULT& lResult, DWORD dwMsgMapID = 0);
 private:
  void ClearTasks();
  inline base::Closure PopTask();
  inline void ExecuteQueuedTasks();
  void ExecuteDelayedTasks();

  
  base::DelayedTaskQueue delayed_tasks_;

  
  std::queue<base::Closure> pending_tasks_;

  
  base::Lock lock_;

  
  HWND wnd_;
  UINT msg_;
};

#endif  
