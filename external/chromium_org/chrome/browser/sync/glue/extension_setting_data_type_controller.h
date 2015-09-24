// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_EXTENSION_SETTING_DATA_TYPE_CONTROLLER_H__
#define CHROME_BROWSER_SYNC_GLUE_EXTENSION_SETTING_DATA_TYPE_CONTROLLER_H__

#include <string>

#include "base/compiler_specific.h"
#include "chrome/browser/sync/glue/non_ui_data_type_controller.h"

class Profile;
class ProfileSyncComponentsFactory;
class ProfileSyncService;

namespace syncer {
class SyncableService;
}

namespace extensions {
class SettingsFrontend;
}

namespace browser_sync {

class ExtensionSettingDataTypeController
    : public NonUIDataTypeController {
 public:
  ExtensionSettingDataTypeController(
      
      syncer::ModelType type,
      ProfileSyncComponentsFactory* profile_sync_factory,
      Profile* profile,
      ProfileSyncService* profile_sync_service);

  
  virtual syncer::ModelType type() const OVERRIDE;
  virtual syncer::ModelSafeGroup model_safe_group() const OVERRIDE;

 private:
  virtual ~ExtensionSettingDataTypeController();

  
  virtual bool PostTaskOnBackendThread(
      const tracked_objects::Location& from_here,
      const base::Closure& task) OVERRIDE;
  virtual bool StartModels() OVERRIDE;

  
  syncer::ModelType type_;

  
  Profile* profile_;
  ProfileSyncService* profile_sync_service_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionSettingDataTypeController);
};

}  

#endif  
