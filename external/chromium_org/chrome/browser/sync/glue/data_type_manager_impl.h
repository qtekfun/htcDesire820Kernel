// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_MANAGER_IMPL_H__
#define CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_MANAGER_IMPL_H__

#include "chrome/browser/sync/glue/data_type_manager.h"

#include <map>
#include <queue>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "base/time/time.h"
#include "chrome/browser/sync/glue/backend_data_type_configurer.h"
#include "chrome/browser/sync/glue/model_association_manager.h"

namespace syncer {
struct DataTypeConfigurationStats;
class DataTypeDebugInfoListener;
template <typename T> class WeakHandle;
}

namespace browser_sync {

class DataTypeController;
class DataTypeEncryptionHandler;
class DataTypeManagerObserver;
class FailedDataTypesHandler;

typedef std::queue<syncer::ModelTypeSet> TypeSetPriorityList;

class DataTypeManagerImpl : public DataTypeManager,
                            public ModelAssociationResultProcessor {
 public:
  DataTypeManagerImpl(
      const syncer::WeakHandle<syncer::DataTypeDebugInfoListener>&
          debug_info_listener,
      const DataTypeController::TypeMap* controllers,
      const DataTypeEncryptionHandler* encryption_handler,
      BackendDataTypeConfigurer* configurer,
      DataTypeManagerObserver* observer,
      FailedDataTypesHandler* failed_data_types_handler);
  virtual ~DataTypeManagerImpl();

  
  virtual void Configure(syncer::ModelTypeSet desired_types,
                         syncer::ConfigureReason reason) OVERRIDE;

  
  virtual void PurgeForMigration(
      syncer::ModelTypeSet undesired_types,
      syncer::ConfigureReason reason) OVERRIDE;

  virtual void Stop() OVERRIDE;
  virtual State state() const OVERRIDE;

  
  virtual void OnSingleDataTypeAssociationDone(
      syncer::ModelType type,
      const syncer::DataTypeAssociationStats& association_stats) OVERRIDE;
  virtual void OnModelAssociationDone(
      const DataTypeManager::ConfigureResult& result) OVERRIDE;

  
  
  ModelAssociationManager* GetModelAssociationManagerForTesting() {
    return &model_association_manager_;
  }

 private:
  friend class TestDataTypeManager;

  
  void Abort(ConfigureStatus status,
             const syncer::SyncError& error);

  
  
  virtual syncer::ModelTypeSet GetPriorityTypes() const;

  
  
  TypeSetPriorityList PrioritizeTypes(const syncer::ModelTypeSet& types);

  
  void ProcessReconfigure();

  void Restart(syncer::ConfigureReason reason);
  void DownloadReady(base::Time download_start_time,
                     syncer::ModelTypeSet types_to_download,
                     syncer::ModelTypeSet high_priority_types_before,
                     syncer::ModelTypeSet first_sync_types,
                     syncer::ModelTypeSet failed_configuration_types);

  
  
  void OnDownloadRetry();
  void NotifyStart();
  void NotifyDone(const ConfigureResult& result);

  
  
  void AddToConfigureTime();

  void ConfigureImpl(syncer::ModelTypeSet desired_types,
                     syncer::ConfigureReason reason);

  BackendDataTypeConfigurer::DataTypeConfigStateMap
  BuildDataTypeConfigStateMap(
      const syncer::ModelTypeSet& types_being_configured) const;

  
  
  void StartNextAssociation();

  void StopImpl();

  BackendDataTypeConfigurer* configurer_;
  
  
  const DataTypeController::TypeMap* controllers_;
  State state_;
  std::map<syncer::ModelType, int> start_order_;
  syncer::ModelTypeSet last_requested_types_;

  
  
  bool needs_reconfigure_;

  
  
  syncer::ConfigureReason last_configure_reason_;

  
  base::Time last_restart_time_;

  
  
  base::TimeDelta configure_time_delta_;

  
  
  const syncer::WeakHandle<syncer::DataTypeDebugInfoListener>
      debug_info_listener_;

  
  ModelAssociationManager model_association_manager_;

  
  
  DataTypeManagerObserver* const observer_;

  
  
  browser_sync::FailedDataTypesHandler* failed_data_types_handler_;

  
  TypeSetPriorityList download_types_queue_;

  
  struct AssociationTypesInfo {
    AssociationTypesInfo();
    ~AssociationTypesInfo();
    syncer::ModelTypeSet types;
    syncer::ModelTypeSet first_sync_types;
    base::Time download_start_time;
    base::Time download_ready_time;
    base::Time association_request_time;
    syncer::ModelTypeSet high_priority_types_before;
    syncer::ModelTypeSet configured_types;
  };
  std::queue<AssociationTypesInfo> association_types_queue_;

  
  
  const browser_sync::DataTypeEncryptionHandler* encryption_handler_;

  
  std::vector<syncer::DataTypeConfigurationStats> configuration_stats_;

  base::WeakPtrFactory<DataTypeManagerImpl> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DataTypeManagerImpl);
};

}  

#endif  
