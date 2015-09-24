// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SERVICE_WORKER_SERVICE_WORKER_REGISTRATION_STATUS_H_
#define CONTENT_BROWSER_SERVICE_WORKER_SERVICE_WORKER_REGISTRATION_STATUS_H_

#include "base/strings/string16.h"
#include "third_party/WebKit/public/platform/WebServiceWorkerError.h"

namespace content {

enum ServiceWorkerRegistrationStatus {
  REGISTRATION_OK,
  REGISTRATION_INSTALL_FAILED,
  REGISTRATION_ACTIVATE_FAILED,
};

void GetServiceWorkerRegistrationStatusResponse(
    ServiceWorkerRegistrationStatus status,
    blink::WebServiceWorkerError::ErrorType* error_type,
    base::string16* message);

}  

#endif  
