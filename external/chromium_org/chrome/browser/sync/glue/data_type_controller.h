// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_CONTROLLER_H__
#define CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_CONTROLLER_H__

#include <map>
#include <string>

#include "base/callback.h"
#include "base/location.h"
#include "base/sequenced_task_runner_helpers.h"
#include "chrome/browser/sync/glue/data_type_error_handler.h"
#include "content/public/browser/browser_thread.h"
#include "sync/api/sync_merge_result.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/engine/model_safe_worker.h"
#include "sync/internal_api/public/util/unrecoverable_error_handler.h"

namespace syncer {
class SyncError;
}

namespace browser_sync {

class DataTypeController
    : public base::RefCountedThreadSafe<
          DataTypeController, content::BrowserThread::DeleteOnUIThread>,
      public DataTypeErrorHandler {
 public:
  enum State {
    NOT_RUNNING,    
                    
    MODEL_STARTING, 
                    
                    
    MODEL_LOADED,   
                    
    ASSOCIATING,    
    RUNNING,        
                    
    STOPPING,       
                    
    DISABLED        
                    
  };

  enum StartResult {
    OK,                   
    OK_FIRST_RUN,         
                          
                          
    ASSOCIATION_FAILED,   
    ABORTED,              
    UNRECOVERABLE_ERROR,  
    NEEDS_CRYPTO,         
                          
    MAX_START_RESULT
  };

  typedef base::Callback<void(StartResult,
                              const syncer::SyncMergeResult&,
                              const syncer::SyncMergeResult&)> StartCallback;

  typedef base::Callback<void(syncer::ModelType,
                              syncer::SyncError)> ModelLoadCallback;

  typedef std::map<syncer::ModelType,
                   scoped_refptr<DataTypeController> > TypeMap;
  typedef std::map<syncer::ModelType, DataTypeController::State> StateMap;

  
  
  static bool IsUnrecoverableResult(StartResult result);

  
  static bool IsSuccessfulResult(StartResult result);

  
  
  
  
  
  virtual void LoadModels(const ModelLoadCallback& model_load_callback) = 0;

  
  
  
  virtual void StartAssociating(const StartCallback& start_callback) = 0;

  
  
  
  virtual void Stop() = 0;

  
  virtual syncer::ModelType type() const = 0;

  
  virtual std::string name() const = 0;

  
  
  
  virtual syncer::ModelSafeGroup model_safe_group() const = 0;

  
  virtual State state() const = 0;

  
  
  virtual syncer::SyncError CreateAndUploadError(
      const tracked_objects::Location& location,
      const std::string& message,
      syncer::ModelType type) OVERRIDE;

 protected:
  friend struct content::BrowserThread::DeleteOnThread<
      content::BrowserThread::UI>;
  friend class base::DeleteHelper<DataTypeController>;

  
  
  
  virtual void OnModelLoaded() = 0;

  virtual ~DataTypeController() {}

  
  
  
  virtual void RecordUnrecoverableError(
      const tracked_objects::Location& from_here,
      const std::string& message);
};

}  

#endif  
