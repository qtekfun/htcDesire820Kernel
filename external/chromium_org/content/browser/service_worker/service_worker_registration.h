// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SERVICE_WORKER_SERVICE_WORKER_REGISTRATION_H_
#define CONTENT_BROWSER_SERVICE_WORKER_SERVICE_WORKER_REGISTRATION_H_

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/browser/service_worker/service_worker_version.h"
#include "content/common/content_export.h"
#include "url/gurl.h"

namespace content {

class ServiceWorkerVersion;

class CONTENT_EXPORT ServiceWorkerRegistration
    : NON_EXPORTED_BASE(public base::RefCounted<ServiceWorkerRegistration>) {
 public:
  ServiceWorkerRegistration(const GURL& pattern,
                            const GURL& script_url,
                            int64 registration_id);

  void Shutdown();
  bool is_shutdown() const { return is_shutdown_; }

  int64 id() const { return registration_id_; }
  const GURL& script_url() const { return script_url_; }
  const GURL& pattern() const { return pattern_; }

  ServiceWorkerVersion* active_version() const {
    DCHECK(!is_shutdown_);
    return active_version_.get();
  }

  ServiceWorkerVersion* pending_version() const {
    DCHECK(!is_shutdown_);
    return pending_version_.get();
  }

  void set_active_version(ServiceWorkerVersion* version) {
    DCHECK(!is_shutdown_);
    active_version_ = version;
  }

  void set_pending_version(ServiceWorkerVersion* version) {
    DCHECK(!is_shutdown_);
    pending_version_ = version;
  }

  
  
  void ActivatePendingVersion();

 private:
  virtual ~ServiceWorkerRegistration();
  friend class base::RefCounted<ServiceWorkerRegistration>;

  const GURL pattern_;
  const GURL script_url_;
  const int64 registration_id_;

  scoped_refptr<ServiceWorkerVersion> active_version_;
  scoped_refptr<ServiceWorkerVersion> pending_version_;

  bool is_shutdown_;

  DISALLOW_COPY_AND_ASSIGN(ServiceWorkerRegistration);
};
}  
#endif  
