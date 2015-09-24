// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_MANAGER_H__
#define CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_MANAGER_H__

#include <list>
#include <set>
#include <string>

#include "chrome/browser/sync/glue/data_type_controller.h"
#include "sync/api/sync_error.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/configure_reason.h"

namespace browser_sync {

class DataTypeManager {
 public:
  enum State {
    STOPPED,           
    DOWNLOAD_PENDING,  
                       
                       

    CONFIGURING,       
    RETRYING,          

    CONFIGURED,        
    STOPPING           
  };

  
  
  enum ConfigureStatus {
    UNKNOWN = -1,
    OK,                  
    PARTIAL_SUCCESS,     
    ABORTED,             
                         
    UNRECOVERABLE_ERROR  
  };

  
  struct ConfigureResult {
    ConfigureResult();
    ConfigureResult(ConfigureStatus status,
                    syncer::ModelTypeSet requested_types);
    ConfigureResult(ConfigureStatus status,
                    syncer::ModelTypeSet requested_types,
                    std::map<syncer::ModelType, syncer::SyncError>
                        failed_data_types,
                    syncer::ModelTypeSet unfinished_data_types,
                    syncer::ModelTypeSet needs_crypto);
    ~ConfigureResult();
    ConfigureStatus status;
    syncer::ModelTypeSet requested_types;

    
    std::map<syncer::ModelType, syncer::SyncError> failed_data_types;

    
    
    
    
    syncer::ModelTypeSet unfinished_data_types;

    
    
    syncer::ModelTypeSet needs_crypto;
  };

  virtual ~DataTypeManager() {}

  
  static std::string ConfigureStatusToString(ConfigureStatus status);

  
  
  
  
  
  
  
  
  
  
  
  
  virtual void Configure(syncer::ModelTypeSet desired_types,
                         syncer::ConfigureReason reason) = 0;

  virtual void PurgeForMigration(syncer::ModelTypeSet undesired_types,
                                 syncer::ConfigureReason reason) = 0;

  
  
  
  
  virtual void Stop() = 0;

  
  virtual State state() const = 0;
};

}  

#endif  
