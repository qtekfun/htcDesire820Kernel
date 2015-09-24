// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_MODEL_SAFE_WORKER_H_
#define CHROME_BROWSER_SYNC_ENGINE_MODEL_SAFE_WORKER_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/sync/syncable/model_type.h"

namespace browser_sync {

enum ModelSafeGroup {
  GROUP_PASSIVE = 0,   
                       
                       
  GROUP_UI,            
  GROUP_DB,            
  GROUP_HISTORY,       
                       
  GROUP_PASSWORD,      
                       
                       
  MODEL_SAFE_GROUP_COUNT,
};

std::string ModelSafeGroupToString(ModelSafeGroup group);

class ModelSafeWorker : public base::RefCountedThreadSafe<ModelSafeWorker> {
 public:
  ModelSafeWorker();
  virtual ~ModelSafeWorker();

  
  
  
  virtual void DoWorkAndWaitUntilDone(Callback0::Type* work);

  virtual ModelSafeGroup GetModelSafeGroup();

  
  
  
  
  virtual bool CurrentThreadIsWorkThread();

 private:
  friend class base::RefCountedThreadSafe<ModelSafeWorker>;

  DISALLOW_COPY_AND_ASSIGN(ModelSafeWorker);
};

typedef std::map<syncable::ModelType, ModelSafeGroup>
    ModelSafeRoutingInfo;

ModelSafeGroup GetGroupForModelType(const syncable::ModelType type,
                                    const ModelSafeRoutingInfo& routes);

class ModelSafeWorkerRegistrar {
 public:
  ModelSafeWorkerRegistrar() { }
  
  virtual void GetWorkers(std::vector<ModelSafeWorker*>* out) = 0;

  
  
  
  virtual void GetModelSafeRoutingInfo(ModelSafeRoutingInfo* out) = 0;
 protected:
  virtual ~ModelSafeWorkerRegistrar() {}
 private:
  DISALLOW_COPY_AND_ASSIGN(ModelSafeWorkerRegistrar);
};

}  

#endif  
