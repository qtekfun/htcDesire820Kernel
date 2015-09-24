// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_NON_FRONTEND_DATA_TYPE_CONTROLLER_H__
#define CHROME_BROWSER_SYNC_GLUE_NON_FRONTEND_DATA_TYPE_CONTROLLER_H__

#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "chrome/browser/sync/glue/data_type_controller.h"
#include "chrome/browser/sync/glue/data_type_error_handler.h"
#include "chrome/browser/sync/profile_sync_components_factory.h"

class Profile;
class ProfileSyncService;
class ProfileSyncComponentsFactory;

namespace base {
class TimeDelta;
}

namespace syncer {
class SyncError;
}

namespace browser_sync {

class AssociatorInterface;
class ChangeProcessor;

class NonFrontendDataTypeController : public DataTypeController {
 public:
  
  class BackendComponentsContainer;

  NonFrontendDataTypeController(
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

  
  struct AssociationResult {
    explicit AssociationResult(syncer::ModelType type);
    ~AssociationResult();
    bool needs_crypto;
    bool unrecoverable_error;
    bool sync_has_nodes;
    syncer::SyncError error;
    syncer::SyncMergeResult local_merge_result;
    syncer::SyncMergeResult syncer_merge_result;
    base::TimeDelta association_time;
    ChangeProcessor* change_processor;
    AssociatorInterface* model_associator;
  };
  void AssociationCallback(AssociationResult result);

 protected:
  
  NonFrontendDataTypeController();

  virtual ~NonFrontendDataTypeController();

  
  virtual void OnModelLoaded() OVERRIDE;

  
  
  
  
  
  
  
  virtual bool StartModels();

  
  
  
  
  
  
  virtual bool PostTaskOnBackendThread(
      const tracked_objects::Location& from_here,
      const base::Closure& task) = 0;

  
  
  virtual ProfileSyncComponentsFactory::SyncComponents
      CreateSyncComponents() = 0;

  
  
  virtual void DisconnectProcessor(ChangeProcessor* processor) = 0;

  
  
  virtual void StartDone(
      DataTypeController::StartResult start_result,
      const syncer::SyncMergeResult& local_merge_result,
      const syncer::SyncMergeResult& syncer_merge_result);

  
  virtual void StartDoneImpl(
      DataTypeController::StartResult start_result,
      DataTypeController::State new_state,
      const syncer::SyncMergeResult& local_merge_result,
      const syncer::SyncMergeResult& syncer_merge_result);

  
  
  virtual void DisableImpl(const tracked_objects::Location& from_here,
                           const std::string& message);

  
  virtual void RecordAssociationTime(base::TimeDelta time);
  
  virtual void RecordStartFailure(StartResult result);

  
  ProfileSyncComponentsFactory* profile_sync_factory() const;
  Profile* profile() const;
  ProfileSyncService* profile_sync_service() const;
  void set_start_callback(const StartCallback& callback);
  void set_state(State state);

  virtual AssociatorInterface* associator() const;
  virtual ChangeProcessor* change_processor() const;

  State state_;
  StartCallback start_callback_;
  ModelLoadCallback model_load_callback_;

 private:
  friend class BackendComponentsContainer;
  ProfileSyncComponentsFactory* const profile_sync_factory_;
  Profile* const profile_;
  ProfileSyncService* const profile_sync_service_;

  
  
  scoped_ptr<BackendComponentsContainer> components_container_;

  AssociatorInterface* model_associator_;
  ChangeProcessor* change_processor_;

  base::WeakPtrFactory<NonFrontendDataTypeController> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(NonFrontendDataTypeController);
};

}  

#endif  
