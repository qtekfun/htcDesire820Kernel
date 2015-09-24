// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_GENERIC_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_GLUE_GENERIC_CHANGE_PROCESSOR_H_

#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/sync/glue/change_processor.h"
#include "chrome/browser/sync/glue/data_type_controller.h"
#include "chrome/browser/sync/glue/data_type_error_handler.h"
#include "sync/api/sync_change_processor.h"
#include "sync/api/sync_merge_result.h"

namespace syncer {
class SyncData;
class SyncableService;

typedef std::vector<syncer::SyncData> SyncDataList;
}  

namespace browser_sync {

class GenericChangeProcessor : public ChangeProcessor,
                               public syncer::SyncChangeProcessor,
                               public base::NonThreadSafe {
 public:
  
  GenericChangeProcessor(
      DataTypeErrorHandler* error_handler,
      const base::WeakPtr<syncer::SyncableService>& local_service,
      const base::WeakPtr<syncer::SyncMergeResult>& merge_result,
      syncer::UserShare* user_share);
  virtual ~GenericChangeProcessor();

  
  
  virtual void ApplyChangesFromSyncModel(
      const syncer::BaseTransaction* trans,
      int64 version,
      const syncer::ImmutableChangeRecordList& changes) OVERRIDE;
  
  
  virtual void CommitChangesFromSyncModel() OVERRIDE;

  
  virtual syncer::SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const syncer::SyncChangeList& change_list) OVERRIDE;

  
  
  
  virtual syncer::SyncDataList GetAllSyncData(syncer::ModelType type)
      const OVERRIDE;

  
  
  virtual syncer::SyncError GetAllSyncDataReturnError(
      syncer::ModelType type,
      syncer::SyncDataList* data) const;

  
  virtual int GetSyncCountForType(syncer::ModelType type);

  
  
  virtual bool SyncModelHasUserCreatedNodes(syncer::ModelType type,
                                            bool* has_nodes);
  virtual bool CryptoReadyIfNecessary(syncer::ModelType type);

 protected:
  
  virtual void StartImpl(Profile* profile) OVERRIDE;           
  virtual syncer::UserShare* share_handle() const OVERRIDE;

 private:
  
  const base::WeakPtr<syncer::SyncableService> local_service_;

  
  
  
  
  const base::WeakPtr<syncer::SyncMergeResult> merge_result_;

  
  
  
  
  syncer::SyncChangeList syncer_changes_;

  
  
  
  
  
  syncer::UserShare* const share_handle_;

  DISALLOW_COPY_AND_ASSIGN(GenericChangeProcessor);
};

}  

#endif  
