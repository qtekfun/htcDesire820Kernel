// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_STORE_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_STORE_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "components/policy/core/common/cloud/cloud_policy_validator.h"
#include "components/policy/core/common/policy_map.h"
#include "components/policy/policy_export.h"
#include "policy/proto/device_management_backend.pb.h"

namespace policy {

class CloudExternalDataManager;

class POLICY_EXPORT CloudPolicyStore {
 public:
  
  enum Status {
    
    STATUS_OK,
    
    STATUS_LOAD_ERROR,
    
    STATUS_STORE_ERROR,
    
    STATUS_PARSE_ERROR,
    
    STATUS_SERIALIZE_ERROR,
    
    STATUS_VALIDATION_ERROR,
    
    STATUS_BAD_STATE,
  };

  
  class POLICY_EXPORT Observer {
   public:
    virtual ~Observer();

    
    virtual void OnStoreLoaded(CloudPolicyStore* store) = 0;

    
    virtual void OnStoreError(CloudPolicyStore* store) = 0;
  };

  CloudPolicyStore();
  virtual ~CloudPolicyStore();

  
  
  bool is_initialized() const { return is_initialized_; }

  base::WeakPtr<CloudExternalDataManager> external_data_manager() const {
    return external_data_manager_;
  }

  const PolicyMap& policy_map() const { return policy_map_; }
  bool has_policy() const {
    return policy_.get() != NULL;
  }
  const enterprise_management::PolicyData* policy() const {
    return policy_.get();
  }
  bool is_managed() const {
    return policy_.get() &&
           policy_->state() == enterprise_management::PolicyData::ACTIVE;
  }
  Status status() const { return status_; }
  CloudPolicyValidatorBase::Status validation_status() const {
    return validation_status_;
  }

  
  
  
  
  
  
  
  void Store(
      const enterprise_management::PolicyFetchResponse& policy,
      int64 invalidation_version);

  virtual void Store(
      const enterprise_management::PolicyFetchResponse& policy) = 0;

  
  
  
  
  virtual void Load() = 0;

  
  void AddObserver(Observer* observer);

  
  void RemoveObserver(Observer* observer);

  
  
  int64 invalidation_version() {
    return invalidation_version_;
  }

  
  
  
  void SetExternalDataManager(
      base::WeakPtr<CloudExternalDataManager> external_data_manager);

  
  
  
  
  
  
  void SetPolicyMapForTesting(const PolicyMap& policy_map);

 protected:
  
  void NotifyStoreLoaded();
  void NotifyStoreError();

  
  base::WeakPtr<CloudExternalDataManager> external_data_manager_;

  
  PolicyMap policy_map_;

  
  scoped_ptr<enterprise_management::PolicyData> policy_;

  
  Status status_;

  
  CloudPolicyValidatorBase::Status validation_status_;

  
  int64 invalidation_version_;

 private:
  
  
  bool is_initialized_;

  ObserverList<Observer, true> observers_;

  DISALLOW_COPY_AND_ASSIGN(CloudPolicyStore);
};

}  

#endif  
