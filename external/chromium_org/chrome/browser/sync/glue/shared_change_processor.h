// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_SHARED_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_GLUE_SHARED_CHANGE_PROCESSOR_H_

#include "base/location.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/message_loop/message_loop_proxy.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/sync/glue/data_type_error_handler.h"
#include "sync/api/sync_change_processor.h"
#include "sync/api/sync_error.h"
#include "sync/api/sync_error_factory.h"
#include "sync/api/sync_merge_result.h"
#include "sync/internal_api/public/engine/model_safe_worker.h"

class ProfileSyncComponentsFactory;
class ProfileSyncService;

namespace syncer {
class SyncData;
class SyncableService;

typedef std::vector<syncer::SyncData> SyncDataList;
}  

namespace browser_sync {

class GenericChangeProcessor;
class DataTypeErrorHandler;

class SharedChangeProcessor
    : public base::RefCountedThreadSafe<SharedChangeProcessor> {
 public:
  
  SharedChangeProcessor();

  
  
  
  
  
  virtual base::WeakPtr<syncer::SyncableService> Connect(
    ProfileSyncComponentsFactory* sync_factory,
    ProfileSyncService* sync_service,
    DataTypeErrorHandler* error_handler,
    syncer::ModelType type,
    const base::WeakPtr<syncer::SyncMergeResult>& merge_result);

  
  
  
  
  
  
  
  
  virtual bool Disconnect();

  
  
  virtual int GetSyncCount();
  virtual syncer::SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const syncer::SyncChangeList& change_list);
  virtual syncer::SyncDataList GetAllSyncData(syncer::ModelType type) const;
  virtual syncer::SyncError GetAllSyncDataReturnError(
      syncer::ModelType type,
      syncer::SyncDataList* data) const;
  virtual bool SyncModelHasUserCreatedNodes(bool* has_nodes);
  virtual bool CryptoReadyIfNecessary();

  
  
  
  virtual void ActivateDataType(syncer::ModelSafeGroup model_safe_group);

  virtual syncer::SyncError CreateAndUploadError(
      const tracked_objects::Location& location,
      const std::string& message);

 protected:
  friend class base::RefCountedThreadSafe<SharedChangeProcessor>;
  virtual ~SharedChangeProcessor();

 private:
  
  
  
  
  
  mutable base::Lock monitor_lock_;
  bool disconnected_;

  
  syncer::ModelType type_;

  
  ProfileSyncService* sync_service_;

  
  
  scoped_refptr<base::MessageLoopProxy> backend_loop_;

  
  GenericChangeProcessor* generic_change_processor_;

  DataTypeErrorHandler* error_handler_;

  DISALLOW_COPY_AND_ASSIGN(SharedChangeProcessor);
};

}  

#endif  
