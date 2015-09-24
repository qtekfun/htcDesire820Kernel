// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_BROWSER_THREAD_MODEL_WORKER_H_
#define CHROME_BROWSER_SYNC_GLUE_BROWSER_THREAD_MODEL_WORKER_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "content/public/browser/browser_thread.h"
#include "sync/internal_api/public/engine/model_safe_worker.h"
#include "sync/internal_api/public/util/syncer_error.h"

namespace base {
class WaitableEvent;
}

namespace browser_sync {

class BrowserThreadModelWorker : public syncer::ModelSafeWorker {
 public:
  BrowserThreadModelWorker(content::BrowserThread::ID thread,
                           syncer::ModelSafeGroup group,
                           syncer::WorkerLoopDestructionObserver* observer);

  
  virtual void RegisterForLoopDestruction() OVERRIDE;
  virtual syncer::ModelSafeGroup GetModelSafeGroup() OVERRIDE;

 protected:
  virtual ~BrowserThreadModelWorker();

  virtual syncer::SyncerError DoWorkAndWaitUntilDoneImpl(
      const syncer::WorkCallback& work) OVERRIDE;

  
  
  
  virtual void CallDoWorkAndSignalTask(
      const syncer::WorkCallback& work,
      base::WaitableEvent* done,
      syncer::SyncerError* error) = 0;

 private:
  content::BrowserThread::ID thread_;
  syncer::ModelSafeGroup group_;

  DISALLOW_COPY_AND_ASSIGN(BrowserThreadModelWorker);
};


class DatabaseModelWorker : public BrowserThreadModelWorker {
 public:
  explicit DatabaseModelWorker(syncer::WorkerLoopDestructionObserver* observer);

 protected:
  virtual void CallDoWorkAndSignalTask(
      const syncer::WorkCallback& work,
      base::WaitableEvent* done,
      syncer::SyncerError* error) OVERRIDE;

 private:
  virtual ~DatabaseModelWorker();
};

class FileModelWorker : public BrowserThreadModelWorker {
 public:
  explicit FileModelWorker(syncer::WorkerLoopDestructionObserver* observer);

 protected:
  virtual void CallDoWorkAndSignalTask(
      const syncer::WorkCallback& work,
      base::WaitableEvent* done,
      syncer::SyncerError* error) OVERRIDE;

 private:
  virtual ~FileModelWorker();
};

}  

#endif  
