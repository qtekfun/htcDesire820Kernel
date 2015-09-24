// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_UI_MODEL_WORKER_H_
#define CHROME_BROWSER_SYNC_GLUE_UI_MODEL_WORKER_H_
#pragma once

#include "base/callback.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/condition_variable.h"
#include "base/task.h"
#include "chrome/browser/sync/engine/syncapi.h"
#include "chrome/browser/sync/engine/model_safe_worker.h"

namespace base {
class WaitableEvent;
}

class MessageLoop;

namespace browser_sync {

class UIModelWorker : public browser_sync::ModelSafeWorker {
 public:
  UIModelWorker();
  virtual ~UIModelWorker();

  
  class CallDoWorkAndSignalTask : public Task {
   public:
    CallDoWorkAndSignalTask(Callback0::Type* work,
                            base::WaitableEvent* work_done,
                            UIModelWorker* scheduler)
        : work_(work), work_done_(work_done), scheduler_(scheduler) {
    }
    virtual ~CallDoWorkAndSignalTask() { }

    
    virtual void Run();

   private:
    
    
    Callback0::Type* work_;
    base::WaitableEvent* work_done_;

    
    UIModelWorker* const scheduler_;

    DISALLOW_COPY_AND_ASSIGN(CallDoWorkAndSignalTask);
  };

  
  
  
  
  void Stop();

  
  virtual void DoWorkAndWaitUntilDone(Callback0::Type* work);
  virtual ModelSafeGroup GetModelSafeGroup();
  virtual bool CurrentThreadIsWorkThread();

  
  
  
  
  void OnSyncerShutdownComplete();

  
  
  void OnTaskCompleted() { pending_work_ = NULL; }

 private:
  
  enum State {
    
    
    WORKING,
    
    
    
    
    
    RUNNING_MANUAL_SHUTDOWN_PUMP,
    
    
    STOPPED,
  };

  
  
  State state_;

  
  
  Task* pending_work_;

  
  
  
  bool syncapi_has_shutdown_;

  
  
  
  
  
  
  base::Lock lock_;

  
  
  
  
  
  base::ConditionVariable syncapi_event_;

  DISALLOW_COPY_AND_ASSIGN(UIModelWorker);
};

}  

#endif  
