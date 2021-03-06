// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_EXTENSION_DATA_TYPE_CONTROLLER_H_
#define CHROME_BROWSER_SYNC_GLUE_EXTENSION_DATA_TYPE_CONTROLLER_H_

#include <string>

#include "base/compiler_specific.h"
#include "chrome/browser/sync/glue/generic_change_processor.h"
#include "chrome/browser/sync/glue/ui_data_type_controller.h"

namespace browser_sync {

class ExtensionDataTypeController : public UIDataTypeController {
 public:
  ExtensionDataTypeController(
      syncer::ModelType type,  
      ProfileSyncComponentsFactory* profile_sync_factory,
      Profile* profile,
      ProfileSyncService* sync_service);

 private:
  virtual ~ExtensionDataTypeController();

  
  virtual bool StartModels() OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(ExtensionDataTypeController);
};

}  

#endif  
