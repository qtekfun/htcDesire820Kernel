// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_FRONTEND_DATA_TYPE_CONTROLLER_H__
#define CHROME_BROWSER_SYNC_GLUE_FRONTEND_DATA_TYPE_CONTROLLER_H__

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/sync/glue/data_type_controller.h"
#include "chrome/browser/sync/glue/data_type_error_handler.h"

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

class FrontendDataTypeController : public DataTypeController {
 public:
  FrontendDataTypeController(
      ProfileSyncComponentsFactory* profile_sync_factory,
      Profile* profile,
      ProfileSyncService* sync_service);

  
  virtual void LoadModels(
      const ModelLoadCallback& model_load_callback) OVERRIDE;
  virtual void StartAssociating(const StartCallback& start_callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual syncer::ModelType type() const = 0;
  virtual syncer::ModelSafeGroup model_safe_group() const OVERRIDE;
  virtual std::string name() const OVERRIDE;
  virtual State state() const OVERRIDE;

  
  virtual void OnSingleDatatypeUnrecoverableError(
      const tracked_objects::Location& from_here,
      const std::string& message) OVERRIDE;

 protected:
  friend class FrontendDataTypeControllerMock;

  
  FrontendDataTypeController();
  virtual ~FrontendDataTypeController();

  
  
  
  
  
  
  virtual bool StartModels();

  
  virtual void CreateSyncComponents() = 0;

  
  virtual void OnModelLoaded() OVERRIDE;

  
  
  virtual void CleanUpState();

  
  virtual void StartDone(
      StartResult start_result,
      const syncer::SyncMergeResult& local_merge_result,
      const syncer::SyncMergeResult& syncer_merge_result);

  
  virtual void RecordAssociationTime(base::TimeDelta time);
  
  virtual void RecordStartFailure(StartResult result);

  virtual AssociatorInterface* model_associator() const;
  virtual void set_model_associator(AssociatorInterface* associator);
  virtual ChangeProcessor* change_processor() const;
  virtual void set_change_processor(ChangeProcessor* processor);

  ProfileSyncComponentsFactory* const profile_sync_factory_;
  Profile* const profile_;
  ProfileSyncService* const sync_service_;

  State state_;

  StartCallback start_callback_;
  ModelLoadCallback model_load_callback_;

  
  
  scoped_ptr<AssociatorInterface> model_associator_;
  scoped_ptr<ChangeProcessor> change_processor_;

 private:
  
  
  
  
  
  virtual bool Associate();

  void AbortModelLoad();

  
  
  void CleanUp();

  DISALLOW_COPY_AND_ASSIGN(FrontendDataTypeController);
};

}  

#endif  
