// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_HISTORY_MODEL_WORKER_H_
#define CHROME_BROWSER_SYNC_GLUE_HISTORY_MODEL_WORKER_H_

#include "sync/internal_api/public/engine/model_safe_worker.h"

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/history_db_task.h"
#include "chrome/browser/history/history_service.h"

class HistoryService;

namespace browser_sync {

class HistoryModelWorker : public syncer::ModelSafeWorker {
 public:
  explicit HistoryModelWorker(
      const base::WeakPtr<HistoryService>& history_service,
      syncer::WorkerLoopDestructionObserver* observer);

  
  virtual void RegisterForLoopDestruction() OVERRIDE;
  virtual syncer::ModelSafeGroup GetModelSafeGroup() OVERRIDE;

  
  
  void RegisterOnDBThread();

 protected:
  virtual syncer::SyncerError DoWorkAndWaitUntilDoneImpl(
      const syncer::WorkCallback& work) OVERRIDE;

 private:
  virtual ~HistoryModelWorker();

  const base::WeakPtr<HistoryService> history_service_;
  
  
  CancelableRequestConsumerT<int, 0> cancelable_consumer_;
  DISALLOW_COPY_AND_ASSIGN(HistoryModelWorker);
};

}  

#endif  
