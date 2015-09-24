// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_SCHEMA_REGISTRY_SERVICE_FACTORY_H_
#define CHROME_BROWSER_POLICY_SCHEMA_REGISTRY_SERVICE_FACTORY_H_

#include <map>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "components/browser_context_keyed_service/browser_context_keyed_base_factory.h"

namespace content {
class BrowserContext;
}

namespace policy {

class CombinedSchemaRegistry;
class Schema;
class SchemaRegistryService;

class SchemaRegistryServiceFactory : public BrowserContextKeyedBaseFactory {
 public:
  
  static SchemaRegistryServiceFactory* GetInstance();

  
  
  static SchemaRegistryService* GetForContext(content::BrowserContext* context);

  
  
  
  static scoped_ptr<SchemaRegistryService> CreateForContext(
      content::BrowserContext* context,
      const Schema& chrome_schema,
      CombinedSchemaRegistry* global_registry);

 private:
  friend struct DefaultSingletonTraits<SchemaRegistryServiceFactory>;

  SchemaRegistryServiceFactory();
  virtual ~SchemaRegistryServiceFactory();

  SchemaRegistryService* GetForContextInternal(
      content::BrowserContext* context);

  scoped_ptr<SchemaRegistryService> CreateForContextInternal(
      content::BrowserContext* context,
      const Schema& chrome_schema,
      CombinedSchemaRegistry* global_registry);

  
  virtual void BrowserContextShutdown(
      content::BrowserContext* context) OVERRIDE;
  virtual void BrowserContextDestroyed(
      content::BrowserContext* context) OVERRIDE;
  virtual void SetEmptyTestingFactory(
      content::BrowserContext* context) OVERRIDE;
  virtual void CreateServiceNow(content::BrowserContext* context) OVERRIDE;

  typedef std::map<content::BrowserContext*, SchemaRegistryService*>
      RegistryMap;
  RegistryMap registries_;

  DISALLOW_COPY_AND_ASSIGN(SchemaRegistryServiceFactory);
};

}  

#endif  
