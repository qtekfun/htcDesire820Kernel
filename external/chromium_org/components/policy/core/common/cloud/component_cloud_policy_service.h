// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_COMPONENT_CLOUD_POLICY_SERVICE_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_COMPONENT_CLOUD_POLICY_SERVICE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "components/policy/core/common/cloud/cloud_policy_client.h"
#include "components/policy/core/common/cloud/cloud_policy_core.h"
#include "components/policy/core/common/cloud/cloud_policy_store.h"
#include "components/policy/core/common/policy_bundle.h"
#include "components/policy/core/common/policy_namespace.h"
#include "components/policy/core/common/schema_registry.h"
#include "components/policy/policy_export.h"

namespace base {
class SequencedTaskRunner;
}

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class ExternalPolicyDataFetcherBackend;
class ResourceCache;
class SchemaMap;

class POLICY_EXPORT ComponentCloudPolicyService
    : public CloudPolicyClient::Observer,
      public CloudPolicyCore::Observer,
      public CloudPolicyStore::Observer,
      public SchemaRegistry::Observer,
      public base::NonThreadSafe {
 public:
  class POLICY_EXPORT Delegate {
   public:
    virtual ~Delegate();

    
    
    
    virtual void OnComponentCloudPolicyUpdated() = 0;
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  ComponentCloudPolicyService(
      Delegate* delegate,
      SchemaRegistry* schema_registry,
      CloudPolicyCore* core,
#if !defined(OS_ANDROID) && !defined(OS_IOS)
      scoped_ptr<ResourceCache> cache,
#endif
      scoped_refptr<net::URLRequestContextGetter> request_context,
      scoped_refptr<base::SequencedTaskRunner> backend_task_runner,
      scoped_refptr<base::SequencedTaskRunner> io_task_runner);
  virtual ~ComponentCloudPolicyService();

  
  static bool SupportsDomain(PolicyDomain domain);

  
  
  bool is_initialized() const { return loaded_initial_policy_; }

  
  const PolicyBundle& policy() const { return policy_; }

  
  void ClearCache();

  
  virtual void OnSchemaRegistryReady() OVERRIDE;
  virtual void OnSchemaRegistryUpdated(bool has_new_schemas) OVERRIDE;

  
  virtual void OnCoreConnected(CloudPolicyCore* core) OVERRIDE;
  virtual void OnCoreDisconnecting(CloudPolicyCore* core) OVERRIDE;
  virtual void OnRefreshSchedulerStarted(CloudPolicyCore* core) OVERRIDE;

  
  virtual void OnStoreLoaded(CloudPolicyStore* store) OVERRIDE;
  virtual void OnStoreError(CloudPolicyStore* store) OVERRIDE;

  
  virtual void OnPolicyFetched(CloudPolicyClient* client) OVERRIDE;
  virtual void OnRegistrationStateChanged(CloudPolicyClient* client) OVERRIDE;
  virtual void OnClientError(CloudPolicyClient* client) OVERRIDE;

 private:
#if !defined(OS_ANDROID) && !defined(OS_IOS)
  class Backend;

  void InitializeIfReady();
  void OnBackendInitialized(scoped_ptr<PolicyBundle> initial_policy);
  void SetCurrentSchema();
  void OnPolicyUpdated(scoped_ptr<PolicyBundle> policy);

  Delegate* delegate_;
  SchemaRegistry* schema_registry_;
  CloudPolicyCore* core_;
  scoped_refptr<net::URLRequestContextGetter> request_context_;
  scoped_refptr<base::SequencedTaskRunner> backend_task_runner_;
  scoped_refptr<base::SequencedTaskRunner> io_task_runner_;

  
  
  
  
  
  scoped_ptr<ExternalPolicyDataFetcherBackend>
      external_policy_data_fetcher_backend_;

  
  
  
  
  scoped_ptr<Backend> backend_;

  
  
  
  scoped_refptr<SchemaMap> current_schema_map_;
#endif  

  
  PolicyBundle policy_;

  
  
  
  bool started_loading_initial_policy_;

  
  
  
  bool loaded_initial_policy_;

  
  
  
  bool is_registered_for_cloud_policy_;

  base::WeakPtrFactory<ComponentCloudPolicyService> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ComponentCloudPolicyService);
};

}  

#endif  
