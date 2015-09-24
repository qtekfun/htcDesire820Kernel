// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CONFIGURATION_POLICY_PROVIDER_H_
#define COMPONENTS_POLICY_CORE_COMMON_CONFIGURATION_POLICY_PROVIDER_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "components/policy/core/common/policy_bundle.h"
#include "components/policy/core/common/policy_namespace.h"
#include "components/policy/core/common/schema_registry.h"
#include "components/policy/policy_export.h"

namespace policy {

class POLICY_EXPORT ConfigurationPolicyProvider
    : public SchemaRegistry::Observer {
 public:
  class POLICY_EXPORT Observer {
   public:
    virtual ~Observer();
    virtual void OnUpdatePolicy(ConfigurationPolicyProvider* provider) = 0;
  };

  ConfigurationPolicyProvider();

  
  
  
  
  virtual ~ConfigurationPolicyProvider();

  
  
  
  
  
  
  virtual void Init(SchemaRegistry* registry);

  
  
  
  
  
  virtual void Shutdown();

  
  const PolicyBundle& policies() const { return policy_bundle_; }

  
  
  
  virtual bool IsInitializationComplete(PolicyDomain domain) const;

  
  
  
  
  
  virtual void RefreshPolicies() = 0;

  
  virtual void AddObserver(Observer* observer);
  virtual void RemoveObserver(Observer* observer);

  
  virtual void OnSchemaRegistryUpdated(bool has_new_schemas) OVERRIDE;
  virtual void OnSchemaRegistryReady() OVERRIDE;

 protected:
  
  
  
  void UpdatePolicy(scoped_ptr<PolicyBundle> bundle);

  SchemaRegistry* schema_registry() const;

  const scoped_refptr<SchemaMap>& schema_map() const;

 private:
  
  PolicyBundle policy_bundle_;

  
  bool did_shutdown_;

  SchemaRegistry* schema_registry_;

  ObserverList<Observer, true> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(ConfigurationPolicyProvider);
};

}  

#endif  
