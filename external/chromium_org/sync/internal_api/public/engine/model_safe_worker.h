// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_ENGINE_MODEL_SAFE_WORKER_H_
#define SYNC_INTERNAL_API_PUBLIC_ENGINE_MODEL_SAFE_WORKER_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/util/syncer_error.h"

namespace base {
class DictionaryValue;
}  

namespace syncer {


typedef base::Callback<enum SyncerError(void)> WorkCallback;

enum ModelSafeGroup {
  GROUP_PASSIVE = 0,   
                       
                       
  GROUP_UI,            
  GROUP_DB,            
  GROUP_FILE,          
  GROUP_HISTORY,       
                       
  GROUP_PASSWORD,      
                       
                       
  MODEL_SAFE_GROUP_COUNT,
};

SYNC_EXPORT std::string ModelSafeGroupToString(ModelSafeGroup group);

class WorkerLoopDestructionObserver {
 public:
  virtual void OnWorkerLoopDestroyed(ModelSafeGroup group) = 0;
};

class SYNC_EXPORT ModelSafeWorker
    : public base::RefCountedThreadSafe<ModelSafeWorker>,
      public base::MessageLoop::DestructionObserver {
 public:
  
  
  
  virtual void RegisterForLoopDestruction() = 0;

  
  
  
  virtual void UnregisterForLoopDestruction(
      base::Callback<void(ModelSafeGroup)> unregister_done_callback);

  
  
  SyncerError DoWorkAndWaitUntilDone(const WorkCallback& work);

  
  
  virtual void RequestStop();

  virtual ModelSafeGroup GetModelSafeGroup() = 0;

  
  virtual void WillDestroyCurrentMessageLoop() OVERRIDE;

 protected:
  friend class base::RefCountedThreadSafe<ModelSafeWorker>;

  explicit ModelSafeWorker(WorkerLoopDestructionObserver* observer);
  virtual ~ModelSafeWorker();

  
  
  
  virtual SyncerError DoWorkAndWaitUntilDoneImpl(const WorkCallback& work) = 0;

  base::WaitableEvent* work_done_or_stopped() {
    return &work_done_or_stopped_;
  }

  
  bool IsStopped();

  
  
  void SetWorkingLoopToCurrent();

 private:
  void UnregisterForLoopDestructionAsync(
      base::Callback<void(ModelSafeGroup)> unregister_done_callback);

  
  
  base::Lock stopped_lock_;
  bool stopped_;

  
  
  base::WaitableEvent work_done_or_stopped_;

  
  WorkerLoopDestructionObserver* observer_;

  
  
  base::Lock working_loop_lock_;
  base::MessageLoop* working_loop_;
  base::WaitableEvent working_loop_set_wait_;
};

typedef std::map<ModelType, ModelSafeGroup> ModelSafeRoutingInfo;

SYNC_EXPORT_PRIVATE base::DictionaryValue* ModelSafeRoutingInfoToValue(
    const ModelSafeRoutingInfo& routing_info);

SYNC_EXPORT std::string ModelSafeRoutingInfoToString(
    const ModelSafeRoutingInfo& routing_info);

SYNC_EXPORT ModelTypeSet GetRoutingInfoTypes(
    const ModelSafeRoutingInfo& routing_info);

SYNC_EXPORT ModelSafeGroup GetGroupForModelType(
    const ModelType type,
    const ModelSafeRoutingInfo& routes);

}  

#endif  
