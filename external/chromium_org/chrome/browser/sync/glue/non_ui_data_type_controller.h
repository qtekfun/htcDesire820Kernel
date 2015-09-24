// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_NON_UI_DATA_TYPE_CONTROLLER_H_
#define CHROME_BROWSER_SYNC_GLUE_NON_UI_DATA_TYPE_CONTROLLER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/sync/glue/data_type_controller.h"
#include "chrome/browser/sync/glue/shared_change_processor.h"

class Profile;
class ProfileSyncService;
class ProfileSyncComponentsFactory;

namespace syncer {
class SyncableService;
}

namespace browser_sync {

class NonUIDataTypeController : public DataTypeController {
 public:
  NonUIDataTypeController(
      ProfileSyncComponentsFactory* profile_sync_factory,
      Profile* profile,
      ProfileSyncService* sync_service);

  
  virtual void LoadModels(
      const ModelLoadCallback& model_load_callback) OVERRIDE;
  virtual void StartAssociating(const StartCallback& start_callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual syncer::ModelType type() const = 0;
  virtual syncer::ModelSafeGroup model_safe_group() const = 0;
  virtual std::string name() const OVERRIDE;
  virtual State state() const OVERRIDE;
  virtual void OnSingleDatatypeUnrecoverableError(
      const tracked_objects::Location& from_here,
      const std::string& message) OVERRIDE;

 protected:
  
  NonUIDataTypeController();
  
  virtual ~NonUIDataTypeController();

  
  virtual void OnModelLoaded() OVERRIDE;

  
  
  
  
  
  
  
  virtual bool StartModels();

  
  
  
  virtual void StopModels();

  
  
  
  virtual bool PostTaskOnBackendThread(
      const tracked_objects::Location& from_here,
      const base::Closure& task) = 0;

  
  
  virtual void StartDone(
      DataTypeController::StartResult start_result,
      const syncer::SyncMergeResult& local_merge_result,
      const syncer::SyncMergeResult& syncer_merge_result);

  
  virtual void StartDoneImpl(
      DataTypeController::StartResult start_result,
      DataTypeController::State new_state,
      const syncer::SyncMergeResult& local_merge_result,
      const syncer::SyncMergeResult& syncer_merge_result);

  
  virtual bool StartAssociationAsync();

  
  virtual void RecordAssociationTime(base::TimeDelta time);

  
  virtual void RecordStartFailure(StartResult result);

  Profile* profile() const { return profile_; }

 private:

  
  void StartAssociationWithSharedChangeProcessor(
      const scoped_refptr<SharedChangeProcessor>& shared_change_processor);

  
  
  
  
  void ClearSharedChangeProcessor();

  
  void StopLocalServiceAsync();

  
  void StopLocalService();

  
  void AbortModelLoad();

  
  
  
  void DisableImpl(const tracked_objects::Location& from_here,
                   const std::string& message);

  ProfileSyncComponentsFactory* const profile_sync_factory_;
  Profile* const profile_;
  ProfileSyncService* const sync_service_;

  
  State state_;

  
  StartCallback start_callback_;
  ModelLoadCallback model_load_callback_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  scoped_refptr<SharedChangeProcessor> shared_change_processor_;

  
  
  
  
  
  base::WeakPtr<syncer::SyncableService> local_service_;
};

}  

#endif  
