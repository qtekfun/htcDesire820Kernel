// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_MANAGER_H__
#define CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_MANAGER_H__
#pragma once

#include <set>

#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "chrome/browser/sync/glue/data_type_controller.h"
#include "chrome/browser/sync/syncable/model_type.h"

namespace browser_sync {

class DataTypeManager {
 public:
  enum State {
    STOPPED,           
    DOWNLOAD_PENDING,  
                       
                       

    CONFIGURING,       
    BLOCKED,           
                       

    CONFIGURED,        
    STOPPING           
  };

  enum ConfigureResult {
    OK,                  
    ASSOCIATION_FAILED,  
    ABORTED,             
                         
    UNRECOVERABLE_ERROR  
                         
  };

  typedef std::set<syncable::ModelType> TypeSet;

  
  
  
  
  
  struct ConfigureResultWithErrorLocation {
    ConfigureResult result;
    TypeSet requested_types;
    scoped_ptr<tracked_objects::Location> location;

    ConfigureResultWithErrorLocation();
    ConfigureResultWithErrorLocation(const ConfigureResult& result,
        const tracked_objects::Location& location,
        const TypeSet& requested_types)
        : result(result),
          requested_types(requested_types) {
      this->location.reset(new tracked_objects::Location(
          location.function_name(),
          location.file_name(),
          location.line_number()));
    }

      ~ConfigureResultWithErrorLocation();
  };


  virtual ~DataTypeManager() {}

  
  
  
  
  
  
  
  
  
  
  
  
  virtual void Configure(const TypeSet& desired_types) = 0;

  
  
  
  
  virtual void Stop() = 0;

  
  virtual const DataTypeController::TypeMap& controllers() = 0;

  
  virtual State state() = 0;
};

}  

#endif  
