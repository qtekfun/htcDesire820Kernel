// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_SYNC_BACKEND_REGISTRAR_H_
#define CHROME_BROWSER_SYNC_GLUE_SYNC_BACKEND_REGISTRAR_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/engine/model_safe_worker.h"
#include "sync/internal_api/public/sync_manager.h"

class Profile;

namespace base {
class MessageLoop;
}

namespace syncer {
struct UserShare;
}  

namespace browser_sync {

class ChangeProcessor;
class UIModelWorker;

class SyncBackendRegistrar : public syncer::SyncManager::ChangeDelegate,
                             public syncer::WorkerLoopDestructionObserver {
 public:
  
  
  SyncBackendRegistrar(const std::string& name,
                       Profile* profile,
                       scoped_ptr<base::Thread> sync_thread);

  
  
  
  
  
  
  
  
  
  
  
  virtual ~SyncBackendRegistrar();

  
  
  
  void SetInitialTypes(syncer::ModelTypeSet initial_types);

  
  
  bool IsNigoriEnabled() const;

  
  
  
  
  
  syncer::ModelTypeSet ConfigureDataTypes(
      syncer::ModelTypeSet types_to_add,
      syncer::ModelTypeSet types_to_remove);

  
  
  
  syncer::ModelTypeSet GetLastConfiguredTypes() const;

  
  void RequestWorkerStopOnUIThread();

  
  
  
  void ActivateDataType(syncer::ModelType type,
                        syncer::ModelSafeGroup group,
                        ChangeProcessor* change_processor,
                        syncer::UserShare* user_share);

  
  
  
  void DeactivateDataType(syncer::ModelType type);

  
  
  bool IsTypeActivatedForTest(syncer::ModelType type) const;

  
  
  virtual void OnChangesApplied(
      syncer::ModelType model_type,
      int64 model_version,
      const syncer::BaseTransaction* trans,
      const syncer::ImmutableChangeRecordList& changes) OVERRIDE;
  virtual void OnChangesComplete(syncer::ModelType model_type) OVERRIDE;

  void GetWorkers(std::vector<syncer::ModelSafeWorker*>* out);
  void GetModelSafeRoutingInfo(syncer::ModelSafeRoutingInfo* out);

  
  virtual void OnWorkerLoopDestroyed(syncer::ModelSafeGroup group) OVERRIDE;

  
  scoped_ptr<base::Thread> ReleaseSyncThread();

  
  void Shutdown();

  base::Thread* sync_thread();

 private:
  typedef std::map<syncer::ModelSafeGroup,
      scoped_refptr<syncer::ModelSafeWorker> > WorkerMap;
  typedef std::map<syncer::ModelType, ChangeProcessor*>
      ProcessorMap;

  
  
  void OnWorkerUnregistrationDone(syncer::ModelSafeGroup group);

  void RemoveWorker(syncer::ModelSafeGroup group);

  
  
  ChangeProcessor* GetProcessor(syncer::ModelType type) const;

  
  
  
  ChangeProcessor* GetProcessorUnsafe(syncer::ModelType type) const;

  
  
  bool IsCurrentThreadSafeForModel(
      syncer::ModelType model_type) const;

  
  const std::string name_;

  Profile* const profile_;

  
  mutable base::Lock lock_;

  
  
  
  
  
  
  
  
  
  
  WorkerMap workers_;
  syncer::ModelSafeRoutingInfo routing_info_;

  
  ProcessorMap processors_;

  
  
  syncer::ModelTypeSet last_configured_types_;

  
  std::vector<scoped_refptr<syncer::ModelSafeWorker> > stopped_workers_;

  
  
  
  
  scoped_ptr<base::Thread> sync_thread_;

  DISALLOW_COPY_AND_ASSIGN(SyncBackendRegistrar);
};

}  

#endif  
