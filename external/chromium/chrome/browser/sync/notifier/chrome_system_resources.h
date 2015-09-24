// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_NOTIFIER_CHROME_SYSTEM_RESOURCES_H_
#define CHROME_BROWSER_SYNC_NOTIFIER_CHROME_SYSTEM_RESOURCES_H_
#pragma once

#include <set>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "base/task.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/sync/notifier/state_writer.h"
#include "google/cacheinvalidation/invalidation-client.h"

namespace sync_notifier {

class ChromeSystemResources : public invalidation::SystemResources {
 public:
  explicit ChromeSystemResources(StateWriter* state_writer);

  ~ChromeSystemResources();

  

  virtual invalidation::Time current_time();

  virtual void StartScheduler();

  virtual void StopScheduler();

  virtual void ScheduleWithDelay(invalidation::TimeDelta delay,
                                 invalidation::Closure* task);

  virtual void ScheduleImmediately(invalidation::Closure* task);

  virtual void ScheduleOnListenerThread(invalidation::Closure* task);

  virtual bool IsRunningOnInternalThread();

  virtual void Log(LogLevel level, const char* file, int line,
                   const char* format, ...);

  virtual void WriteState(const invalidation::string& state,
                          invalidation::StorageCallback* callback);

 private:
  base::NonThreadSafe non_thread_safe_;
  scoped_ptr<ScopedRunnableMethodFactory<ChromeSystemResources> >
      scoped_runnable_method_factory_;
  
  std::set<invalidation::Closure*> posted_tasks_;
  StateWriter* state_writer_;

  
  const MessageLoop* created_on_loop_;

  
  
  
  Task* MakeTaskToPost(invalidation::Closure* task);

  
  void RunPostedTask(invalidation::Closure* task);

  
  void RunAndDeleteStorageCallback(
      invalidation::StorageCallback* callback);
};

}  

#endif  
