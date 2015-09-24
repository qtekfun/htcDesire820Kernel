// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_CONTROLLER_H__
#define CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_CONTROLLER_H__
#pragma once

#include <string>
#include <map>

#include "base/callback.h"
#include "base/tracked.h"
#include "chrome/browser/sync/engine/model_safe_worker.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "chrome/browser/sync/unrecoverable_error_handler.h"
#include "content/browser/browser_thread.h"

namespace browser_sync {

class DataTypeController
    : public base::RefCountedThreadSafe<DataTypeController,
                                        BrowserThread::DeleteOnUIThread>,
      public UnrecoverableErrorHandler {
 public:
  enum State {
    NOT_RUNNING,    
                    
    MODEL_STARTING, 
                    
                    
    ASSOCIATING,    
    RUNNING,        
                    
    STOPPING        
                    
  };

  enum StartResult {
    OK,                   
    OK_FIRST_RUN,         
                          
                          
    BUSY,                 
    NOT_ENABLED,          
    ASSOCIATION_FAILED,   
    ABORTED,              
    UNRECOVERABLE_ERROR,  
    NEEDS_CRYPTO,         
                          
    MAX_START_RESULT
  };

  typedef Callback2<StartResult,
      const tracked_objects::Location&>::Type StartCallback;

  typedef std::map<syncable::ModelType,
                   scoped_refptr<DataTypeController> > TypeMap;
  typedef std::map<syncable::ModelType, DataTypeController::State> StateMap;

  
  
  
  
  
  
  virtual void Start(StartCallback* start_callback) = 0;

  
  
  
  
  virtual void Stop() = 0;

  
  virtual syncable::ModelType type() const = 0;

  
  virtual std::string name() const = 0;

  
  
  
  virtual browser_sync::ModelSafeGroup model_safe_group() const = 0;

  
  virtual State state() const = 0;

 protected:
  friend struct BrowserThread::DeleteOnThread<BrowserThread::UI>;
  friend class DeleteTask<DataTypeController>;
  friend class ShutdownTask;

  virtual ~DataTypeController() {}
};

}  

#endif  
