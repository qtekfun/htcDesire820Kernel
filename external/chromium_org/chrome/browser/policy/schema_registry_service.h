// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_SCHEMA_REGISTRY_SERVICE_H_
#define CHROME_BROWSER_POLICY_SCHEMA_REGISTRY_SERVICE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "components/policy/core/common/schema_registry.h"

namespace policy {

class Schema;

class SchemaRegistryService : public SchemaRegistry,
                              public BrowserContextKeyedService {
 public:
  
  
  
  SchemaRegistryService(const Schema& chrome_schema,
                        CombinedSchemaRegistry* global_registry);
  virtual ~SchemaRegistryService();

  
  virtual void Shutdown() OVERRIDE;

 private:
  CombinedSchemaRegistry* global_registry_;

  DISALLOW_COPY_AND_ASSIGN(SchemaRegistryService);
};

}  

#endif  
