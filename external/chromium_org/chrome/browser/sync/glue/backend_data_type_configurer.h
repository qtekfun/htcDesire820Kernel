// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_BACKEND_DATA_TYPE_CONFIGURER_H_
#define CHROME_BROWSER_SYNC_GLUE_BACKEND_DATA_TYPE_CONFIGURER_H_

#include <map>

#include "base/callback.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/configure_reason.h"

namespace browser_sync {

class BackendDataTypeConfigurer {
 public:
  enum DataTypeConfigState {
    CONFIGURE_ACTIVE,     
                          
    CONFIGURE_INACTIVE,   
                          
                          
    CONFIGURE_CLEAN,      
                          
    DISABLED,             
    FATAL,                
    CRYPTO,               
  };
  typedef std::map<syncer::ModelType, DataTypeConfigState>
      DataTypeConfigStateMap;

  
  
  
  
  
  
  
  
  
  
  
  virtual void ConfigureDataTypes(
      syncer::ConfigureReason reason,
      const DataTypeConfigStateMap& config_state_map,
      const base::Callback<void(syncer::ModelTypeSet,
                                syncer::ModelTypeSet)>& ready_task,
      const base::Callback<void()>& retry_callback) = 0;

  
  static syncer::ModelTypeSet GetDataTypesInState(
      DataTypeConfigState state, const DataTypeConfigStateMap& state_map);

  
  static void SetDataTypesState(DataTypeConfigState state,
                                syncer::ModelTypeSet types,
                                DataTypeConfigStateMap* state_map);

 protected:
  virtual ~BackendDataTypeConfigurer() {}
};

}  

#endif  
