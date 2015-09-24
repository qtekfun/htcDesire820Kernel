// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SERVICE_WORKER_SERVICE_WORKER_CONTEXT_WRAPPER_H_
#define CONTENT_BROWSER_SERVICE_WORKER_SERVICE_WORKER_CONTEXT_WRAPPER_H_

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/browser/service_worker/service_worker_context.h"
#include "content/common/content_export.h"

namespace base {
class FilePath;
}

namespace quota {
class QuotaManagerProxy;
}

namespace content {

class ServiceWorkerContextCore;

class CONTENT_EXPORT ServiceWorkerContextWrapper
    : NON_EXPORTED_BASE(public ServiceWorkerContext),
      public base::RefCountedThreadSafe<ServiceWorkerContextWrapper> {
 public:
  ServiceWorkerContextWrapper();

  
  
  void Init(const base::FilePath& user_data_directory,
            quota::QuotaManagerProxy* quota_manager_proxy);
  void Shutdown();

  
  ServiceWorkerContextCore* context();

 private:
  friend class base::RefCountedThreadSafe<ServiceWorkerContextWrapper>;
  virtual ~ServiceWorkerContextWrapper();

  scoped_ptr<ServiceWorkerContextCore> context_core_;
};

}  

#endif  
