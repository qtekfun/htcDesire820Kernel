// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_ERROR_HANDLER_H__
#define CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_ERROR_HANDLER_H__

#include <string>
#include "base/location.h"

#include "sync/api/sync_error.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/util/unrecoverable_error_handler.h"

namespace browser_sync {

class DataTypeErrorHandler {
 public:
  
  
  virtual void OnSingleDatatypeUnrecoverableError(
      const tracked_objects::Location& from_here,
      const std::string& message) = 0;

  
  
  
  virtual syncer::SyncError CreateAndUploadError(
      const tracked_objects::Location& location,
      const std::string& message,
      syncer::ModelType type) = 0;

 protected:
  virtual ~DataTypeErrorHandler() { }
};

}  
#endif  
