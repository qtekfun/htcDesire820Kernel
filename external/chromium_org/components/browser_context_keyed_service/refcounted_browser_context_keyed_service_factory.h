// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_BROWSER_CONTEXT_KEYED_SERVICE_REFCOUNTED_BROWSER_CONTEXT_KEYED_SERVICE_FACTORY_H_
#define COMPONENTS_BROWSER_CONTEXT_KEYED_SERVICE_REFCOUNTED_BROWSER_CONTEXT_KEYED_SERVICE_FACTORY_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_factory.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service_export.h"
#include "components/browser_context_keyed_service/refcounted_browser_context_keyed_service.h"

class RefcountedBrowserContextKeyedService;

namespace content {
class BrowserContext;
}

class BROWSER_CONTEXT_KEYED_SERVICE_EXPORT
RefcountedBrowserContextKeyedServiceFactory
    : public BrowserContextKeyedBaseFactory {
 public:
  
  
  
  typedef scoped_refptr<RefcountedBrowserContextKeyedService>
      (*TestingFactoryFunction)(content::BrowserContext* context);

  
  
  
  
  void SetTestingFactory(content::BrowserContext* context,
                         TestingFactoryFunction factory);

  
  
  
  scoped_refptr<RefcountedBrowserContextKeyedService> SetTestingFactoryAndUse(
      content::BrowserContext* context,
      TestingFactoryFunction factory);

 protected:
  RefcountedBrowserContextKeyedServiceFactory(
      const char* name,
      BrowserContextDependencyManager* manager);
  virtual ~RefcountedBrowserContextKeyedServiceFactory();

  scoped_refptr<RefcountedBrowserContextKeyedService>
      GetServiceForBrowserContext(
          content::BrowserContext* context,
          bool create);

  
  void Associate(
      content::BrowserContext* context,
      const scoped_refptr<RefcountedBrowserContextKeyedService>& service);

  
  
  
  virtual scoped_refptr<RefcountedBrowserContextKeyedService>
      BuildServiceInstanceFor(content::BrowserContext* context) const = 0;

  virtual void BrowserContextShutdown(
      content::BrowserContext* context) OVERRIDE;
  virtual void BrowserContextDestroyed(
      content::BrowserContext* context) OVERRIDE;
  virtual void SetEmptyTestingFactory(
      content::BrowserContext* context) OVERRIDE;
  virtual void CreateServiceNow(content::BrowserContext* context) OVERRIDE;

 private:
  typedef std::map<content::BrowserContext*,
                   scoped_refptr<RefcountedBrowserContextKeyedService> >
      RefCountedStorage;
  typedef std::map<content::BrowserContext*, TestingFactoryFunction>
      BrowserContextOverriddenTestingFunctions;

  
  RefCountedStorage mapping_;

  
  
  BrowserContextOverriddenTestingFunctions testing_factories_;

  DISALLOW_COPY_AND_ASSIGN(RefcountedBrowserContextKeyedServiceFactory);
};

#endif  
