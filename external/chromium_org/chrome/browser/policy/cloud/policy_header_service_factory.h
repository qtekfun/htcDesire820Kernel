// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_CLOUD_POLICY_HEADER_SERVICE_FACTORY_H_
#define CHROME_BROWSER_POLICY_CLOUD_POLICY_HEADER_SERVICE_FACTORY_H_

#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"

namespace policy {

class PolicyHeaderService;

class PolicyHeaderServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  
  
  static PolicyHeaderService* GetForBrowserContext(
      content::BrowserContext* context);

  
  static PolicyHeaderServiceFactory* GetInstance();

 protected:
  
  virtual BrowserContextKeyedService* BuildServiceInstanceFor(
      content::BrowserContext* profile) const OVERRIDE;

 private:
  friend struct DefaultSingletonTraits<PolicyHeaderServiceFactory>;

  PolicyHeaderServiceFactory();
  virtual ~PolicyHeaderServiceFactory();

  DISALLOW_COPY_AND_ASSIGN(PolicyHeaderServiceFactory);
};

}  

#endif  
