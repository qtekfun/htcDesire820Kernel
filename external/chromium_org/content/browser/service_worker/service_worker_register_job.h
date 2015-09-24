// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SERVICE_WORKER_SERVICE_WORKER_REGISTER_JOB_H_
#define CONTENT_BROWSER_SERVICE_WORKER_SERVICE_WORKER_REGISTER_JOB_H_

#include "base/memory/weak_ptr.h"
#include "content/browser/service_worker/service_worker_registration_status.h"
#include "content/browser/service_worker/service_worker_storage.h"

namespace content {

class ServiceWorkerRegisterJob {
 public:
  typedef base::Callback<void(
      ServiceWorkerRegisterJob* job,
      ServiceWorkerRegistrationStatus status,
      ServiceWorkerRegistration* registration)> RegistrationCompleteCallback;

  
  
  ServiceWorkerRegisterJob(const base::WeakPtr<ServiceWorkerStorage>& storage,
                           const RegistrationCompleteCallback& callback);
  ~ServiceWorkerRegisterJob();

  
  
  
  
  
  
  
  
  
  
  
  
  void StartRegister(const GURL& pattern, const GURL& script_url);

  
  
  
  
  void StartUnregister(const GURL& pattern);

 private:
  
  void RegisterPatternAndContinue(
      const GURL& pattern,
      const GURL& script_url,
      const ServiceWorkerStorage::RegistrationCallback& callback,
      ServiceWorkerRegistrationStatus previous_status);

  void UnregisterPatternAndContinue(
      const GURL& pattern,
      const GURL& script_url,
      const ServiceWorkerStorage::UnregistrationCallback& callback,
      bool found,
      ServiceWorkerRegistrationStatus previous_status,
      const scoped_refptr<ServiceWorkerRegistration>& previous_registration);

  
  
  void UnregisterComplete(ServiceWorkerRegistrationStatus status);
  void RegisterComplete(
      ServiceWorkerRegistrationStatus status,
      const scoped_refptr<ServiceWorkerRegistration>& registration);

  const base::WeakPtr<ServiceWorkerStorage> storage_;
  const RegistrationCompleteCallback callback_;
  base::WeakPtrFactory<ServiceWorkerRegisterJob> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(ServiceWorkerRegisterJob);
};
}  

#endif  
