// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_MANAGER_IMPL_H__
#define CHROME_BROWSER_SYNC_GLUE_DATA_TYPE_MANAGER_IMPL_H__
#pragma once

#include "chrome/browser/sync/glue/data_type_manager.h"

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/task.h"

namespace browser_sync {

class DataTypeController;
class SyncBackendHost;

class DataTypeManagerImpl : public DataTypeManager {
 public:
  DataTypeManagerImpl(SyncBackendHost* backend,
                       const DataTypeController::TypeMap& controllers);
  virtual ~DataTypeManagerImpl();

  
  virtual void Configure(const TypeSet& desired_types);
  virtual void Stop();
  virtual const DataTypeController::TypeMap& controllers();
  virtual State state();

 private:
  
  
  
  void StartNextType();

  
  void TypeStartCallback(DataTypeController::StartResult result,
      const tracked_objects::Location& from_here);

  
  void FinishStop();
  void FinishStopAndNotify(ConfigureResult result,
       const tracked_objects::Location& location);

  
  
  bool GetControllersNeedingStart(
      std::vector<DataTypeController*>* needs_start);

  void Restart();
  void DownloadReady();
  void NotifyStart();
  void NotifyDone(ConfigureResult result,
      const tracked_objects::Location& location);
  void SetBlockedAndNotify();

  SyncBackendHost* backend_;
  
  
  const DataTypeController::TypeMap controllers_;
  State state_;
  std::map<syncable::ModelType, int> start_order_;
  TypeSet last_requested_types_;
  std::vector<DataTypeController*> needs_start_;
  std::vector<DataTypeController*> needs_stop_;

  
  
  bool needs_reconfigure_;

  ScopedRunnableMethodFactory<DataTypeManagerImpl> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(DataTypeManagerImpl);
};

}  

#endif  
