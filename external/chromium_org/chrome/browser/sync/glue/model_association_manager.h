// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_MODEL_ASSOCIATION_MANAGER_H__
#define CHROME_BROWSER_SYNC_GLUE_MODEL_ASSOCIATION_MANAGER_H__

#include <map>

#include "base/memory/weak_ptr.h"
#include "base/timer/timer.h"

#include "chrome/browser/sync/glue/data_type_manager.h"
#include "sync/internal_api/public/data_type_association_stats.h"
#include "sync/internal_api/public/util/weak_handle.h"

namespace browser_sync {

class DataTypeController;

class ModelAssociationResultProcessor {
 public:
  virtual void OnSingleDataTypeAssociationDone(
      syncer::ModelType type,
      const syncer::DataTypeAssociationStats& association_stats) = 0;
  virtual void OnModelAssociationDone(
      const DataTypeManager::ConfigureResult& result) = 0;
  virtual ~ModelAssociationResultProcessor() {}
};

class ModelAssociationManager {
 public:
  ModelAssociationManager(const DataTypeController::TypeMap* controllers,
                          ModelAssociationResultProcessor* processor);
  virtual ~ModelAssociationManager();

  
  
  
  
  
  
  void Initialize(syncer::ModelTypeSet desired_types);

  
  void Stop();

  
  
  
  void StartAssociationAsync(const syncer::ModelTypeSet& types_to_associate);

  
  
  
  
   base::OneShotTimer<ModelAssociationManager>* GetTimerForTesting();

 private:
  enum State {
    
    INITIALIZED_TO_CONFIGURE,
    
    CONFIGURING,
    
    IDLE
  };

  
  
  void ResetForNextAssociation();

  
  void StopDisabledTypes();

  
  void LoadEnabledTypes();

  
  
  void TypeStartCallback(syncer::ModelType type,
                         base::TimeTicks type_start_time,
                         DataTypeController::StartResult start_result,
                         const syncer::SyncMergeResult& local_merge_result,
                         const syncer::SyncMergeResult& syncer_merge_result);

  
  
  void ModelLoadCallback(syncer::ModelType type, syncer::SyncError error);

  
  
  void AppendToFailedDatatypesAndLogError(const syncer::SyncError& error);

  
  
  void ModelAssociationDone();

  State state_;

  
  syncer::ModelTypeSet desired_types_;

  
  syncer::ModelTypeSet requested_types_;

  
  
  syncer::ModelTypeSet associating_types_;

  
  syncer::ModelTypeSet loaded_types_;

  
  
  syncer::ModelTypeSet associated_types_;

  
  
  syncer::ModelTypeSet slow_types_;

  
  
  
  std::map<syncer::ModelType, syncer::SyncError> failed_data_types_info_;

  
  syncer::ModelTypeSet needs_crypto_types_;

  
  
  base::TimeTicks association_start_time_;

  
  const DataTypeController::TypeMap* controllers_;

  
  ModelAssociationResultProcessor* result_processor_;

  
  base::OneShotTimer<ModelAssociationManager> timer_;

  base::WeakPtrFactory<ModelAssociationManager> weak_ptr_factory_;

  DataTypeManager::ConfigureStatus configure_status_;

  DISALLOW_COPY_AND_ASSIGN(ModelAssociationManager);
};
}  
#endif  
