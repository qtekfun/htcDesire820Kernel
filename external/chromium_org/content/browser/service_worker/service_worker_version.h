// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SERVICE_WORKER_SERVICE_WORKER_VERSION_H_
#define CONTENT_BROWSER_SERVICE_WORKER_SERVICE_WORKER_VERSION_H_

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/common/content_export.h"

class GURL;

namespace content {

class EmbeddedWorkerInstance;
class EmbeddedWorkerRegistry;
class ServiceWorkerProviderHost;
class ServiceWorkerRegistration;

class CONTENT_EXPORT ServiceWorkerVersion
    : NON_EXPORTED_BASE(public base::RefCounted<ServiceWorkerVersion>) {
 public:
  ServiceWorkerVersion(
      ServiceWorkerRegistration* registration,
      EmbeddedWorkerRegistry* worker_registry,
      int64 version_id);

  int64 version_id() const { return version_id_; }

  void Shutdown();
  bool is_shutdown() const { return is_shutdown_; }

  
  void StartWorker();
  void StopWorker();

  
  
  void OnAssociateProvider(ServiceWorkerProviderHost* provider_host);
  void OnUnassociateProvider(ServiceWorkerProviderHost* provider_host);

 private:
  friend class base::RefCounted<ServiceWorkerVersion>;

  ~ServiceWorkerVersion();

  const int64 version_id_;

  bool is_shutdown_;
  scoped_refptr<ServiceWorkerRegistration> registration_;

  scoped_ptr<EmbeddedWorkerInstance> embedded_worker_;

  DISALLOW_COPY_AND_ASSIGN(ServiceWorkerVersion);
};

}  

#endif  
