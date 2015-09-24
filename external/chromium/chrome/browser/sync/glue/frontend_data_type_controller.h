// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_FRONTEND_DATA_TYPE_CONTROLLER_H__
#define CHROME_BROWSER_SYNC_GLUE_FRONTEND_DATA_TYPE_CONTROLLER_H__
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/sync/glue/data_type_controller.h"

class Profile;
class ProfileSyncService;
class ProfileSyncFactory;

namespace base { class TimeDelta; }
namespace browser_sync {

class AssociatorInterface;
class ChangeProcessor;

class FrontendDataTypeController : public DataTypeController {
 public:
  FrontendDataTypeController(
      ProfileSyncFactory* profile_sync_factory,
      Profile* profile,
      ProfileSyncService* sync_service);
  virtual ~FrontendDataTypeController();

  
  virtual void Start(StartCallback* start_callback);

  virtual void Stop();

  virtual syncable::ModelType type() const = 0;

  virtual browser_sync::ModelSafeGroup model_safe_group() const;

  virtual std::string name() const;

  virtual State state() const;

  
  virtual void OnUnrecoverableError(const tracked_objects::Location& from_here,
                                    const std::string& message);
 protected:
  
  FrontendDataTypeController();

  
  
  virtual bool StartModels();

  
  virtual bool Associate();

  
  
  virtual void CleanupState();

  
  virtual void FinishStart(StartResult result,
      const tracked_objects::Location& from_here);

  
  virtual void StartFailed(StartResult result,
      const tracked_objects::Location& from_here);

  
  virtual void CreateSyncComponents() = 0;

  
  
  
  virtual void RecordUnrecoverableError(
      const tracked_objects::Location& from_here,
      const std::string& message) = 0;
  
  virtual void RecordAssociationTime(base::TimeDelta time) = 0;
  
  virtual void RecordStartFailure(StartResult result) = 0;

  ProfileSyncFactory* const profile_sync_factory_;
  Profile* const profile_;
  ProfileSyncService* const sync_service_;

  State state_;

  scoped_ptr<StartCallback> start_callback_;
  scoped_ptr<AssociatorInterface> model_associator_;
  scoped_ptr<ChangeProcessor> change_processor_;

  DISALLOW_COPY_AND_ASSIGN(FrontendDataTypeController);
};

}  

#endif  
