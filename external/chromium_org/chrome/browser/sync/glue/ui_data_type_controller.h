// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_UI_DATA_TYPE_CONTROLLER_H__
#define CHROME_BROWSER_SYNC_GLUE_UI_DATA_TYPE_CONTROLLER_H__

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

namespace base {
class TimeDelta;
}

namespace syncer {
class SyncableService;
class SyncError;
}

namespace browser_sync {

class UIDataTypeController : public DataTypeController {
 public:
  UIDataTypeController(
      syncer::ModelType type,
      ProfileSyncComponentsFactory* profile_sync_factory,
      Profile* profile,
      ProfileSyncService* sync_service);

  
  virtual void LoadModels(
      const ModelLoadCallback& model_load_callback) OVERRIDE;
  virtual void StartAssociating(const StartCallback& start_callback) OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual syncer::ModelType type() const OVERRIDE;
  virtual syncer::ModelSafeGroup model_safe_group() const OVERRIDE;
  virtual std::string name() const OVERRIDE;
  virtual State state() const OVERRIDE;

  
  virtual void OnSingleDatatypeUnrecoverableError(
      const tracked_objects::Location& from_here,
      const std::string& message) OVERRIDE;

 protected:
  
  UIDataTypeController();
  
  virtual ~UIDataTypeController();

  
  
  
  
  
  
  virtual bool StartModels();

  
  
  virtual void StopModels();

  
  virtual void OnModelLoaded() OVERRIDE;

  
  virtual void StartDone(StartResult result,
                         const syncer::SyncMergeResult& local_merge_result,
                         const syncer::SyncMergeResult& syncer_merge_result);

  
  virtual void RecordAssociationTime(base::TimeDelta time);
  
  virtual void RecordStartFailure(StartResult result);

  ProfileSyncComponentsFactory* const profile_sync_factory_;
  Profile* const profile_;
  ProfileSyncService* const sync_service_;

  State state_;

  StartCallback start_callback_;
  ModelLoadCallback model_load_callback_;

  
  syncer::ModelType type_;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  scoped_refptr<SharedChangeProcessor> shared_change_processor_;

  
  
  base::WeakPtr<syncer::SyncableService> local_service_;

 private:
   
  virtual void Associate();

  virtual void AbortModelLoad();

  DISALLOW_COPY_AND_ASSIGN(UIDataTypeController);
};

}  

#endif  
