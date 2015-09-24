// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_BROWSER_CONTEXT_KEYED_SERVICE_BROWSER_CONTEXT_KEYED_SERVICE_H_
#define COMPONENTS_BROWSER_CONTEXT_KEYED_SERVICE_BROWSER_CONTEXT_KEYED_SERVICE_H_

#include "components/browser_context_keyed_service/browser_context_keyed_service_export.h"

class BrowserContextKeyedServiceFactory;

class BROWSER_CONTEXT_KEYED_SERVICE_EXPORT BrowserContextKeyedService {
 public:
  
  virtual void Shutdown() {}

 protected:
  friend class BrowserContextKeyedServiceFactory;

  
  virtual ~BrowserContextKeyedService() {}
};

#endif  
