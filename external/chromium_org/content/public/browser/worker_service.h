// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_WORKER_SERVICE_H_
#define CONTENT_PUBLIC_BROWSER_WORKER_SERVICE_H_

#include <vector>

#include "base/process/process.h"
#include "content/common/content_export.h"
#include "url/gurl.h"

namespace content {

class WorkerServiceObserver;

class WorkerService {
 public:
  virtual ~WorkerService() {}

  
  CONTENT_EXPORT static WorkerService* GetInstance();

  
  virtual bool TerminateWorker(int process_id, int route_id) = 0;

  struct WorkerInfo {
    GURL url;
    base::string16 name;
    int process_id;
    int route_id;
    base::ProcessHandle handle;
  };

  
  virtual std::vector<WorkerInfo> GetWorkers() = 0;

  virtual void AddObserver(WorkerServiceObserver* observer) = 0;
  virtual void RemoveObserver(WorkerServiceObserver* observer) = 0;
};

}  

#endif  
