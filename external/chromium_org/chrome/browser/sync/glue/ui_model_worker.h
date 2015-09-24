// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_UI_MODEL_WORKER_H_
#define CHROME_BROWSER_SYNC_GLUE_UI_MODEL_WORKER_H_

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/synchronization/condition_variable.h"
#include "base/synchronization/lock.h"
#include "sync/internal_api/public/engine/model_safe_worker.h"
#include "sync/internal_api/public/util/unrecoverable_error_info.h"

namespace browser_sync {

class UIModelWorker : public syncer::ModelSafeWorker {
 public:
  explicit UIModelWorker(syncer::WorkerLoopDestructionObserver* observer);

  
  virtual void RegisterForLoopDestruction() OVERRIDE;
  virtual syncer::ModelSafeGroup GetModelSafeGroup() OVERRIDE;

 protected:
  virtual syncer::SyncerError DoWorkAndWaitUntilDoneImpl(
      const syncer::WorkCallback& work) OVERRIDE;

 private:
  virtual ~UIModelWorker();

  DISALLOW_COPY_AND_ASSIGN(UIModelWorker);
};

}  

#endif  
