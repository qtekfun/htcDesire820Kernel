// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_SERVICE_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_SERVICE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "components/policy/core/common/cloud/cloud_policy_client.h"
#include "components/policy/core/common/cloud/cloud_policy_constants.h"
#include "components/policy/core/common/cloud/cloud_policy_store.h"
#include "components/policy/policy_export.h"

namespace policy {

class POLICY_EXPORT CloudPolicyService : public CloudPolicyClient::Observer,
                                         public CloudPolicyStore::Observer {
 public:
  
  
  typedef base::Callback<void(bool)> RefreshPolicyCallback;

  class POLICY_EXPORT Observer {
   public:
    
    
    
    virtual void OnInitializationCompleted(CloudPolicyService* service) = 0;
    virtual ~Observer() {}
  };

  
  CloudPolicyService(const PolicyNamespaceKey& policy_ns_key,
                     CloudPolicyClient* client,
                     CloudPolicyStore* store);
  virtual ~CloudPolicyService();

  
  
  std::string ManagedBy() const;

  
  
  void RefreshPolicy(const RefreshPolicyCallback& callback);

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  virtual void OnPolicyFetched(CloudPolicyClient* client) OVERRIDE;
  virtual void OnRegistrationStateChanged(CloudPolicyClient* client) OVERRIDE;
  virtual void OnClientError(CloudPolicyClient* client) OVERRIDE;

  
  virtual void OnStoreLoaded(CloudPolicyStore* store) OVERRIDE;
  virtual void OnStoreError(CloudPolicyStore* store) OVERRIDE;

  bool IsInitializationComplete() const { return initialization_complete_; }

 private:
  
  
  void CheckInitializationCompleted();

  
  
  void RefreshCompleted(bool success);

  
  PolicyNamespaceKey policy_ns_key_;

  
  CloudPolicyClient* client_;

  
  CloudPolicyStore* store_;

  
  enum {
    
    REFRESH_NONE,
    
    REFRESH_POLICY_FETCH,
    
    REFRESH_POLICY_STORE,
  } refresh_state_;

  
  std::vector<RefreshPolicyCallback> refresh_callbacks_;

  
  
  bool initialization_complete_;

  
  
  ObserverList<Observer, true> observers_;

  DISALLOW_COPY_AND_ASSIGN(CloudPolicyService);
};

}  

#endif  
