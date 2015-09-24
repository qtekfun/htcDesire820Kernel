// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_POLICY_SERVICE_H_
#define COMPONENTS_POLICY_CORE_COMMON_POLICY_SERVICE_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "components/policy/core/common/policy_map.h"
#include "components/policy/core/common/policy_namespace.h"
#include "components/policy/policy_export.h"

namespace policy {

class POLICY_EXPORT PolicyService {
 public:
  class POLICY_EXPORT Observer {
   public:
    
    
    
    
    virtual void OnPolicyUpdated(const PolicyNamespace& ns,
                                 const PolicyMap& previous,
                                 const PolicyMap& current) = 0;

    
    
    
    
    virtual void OnPolicyServiceInitialized(PolicyDomain domain) {}

   protected:
    virtual ~Observer() {}
  };

  virtual ~PolicyService() {}

  
  virtual void AddObserver(PolicyDomain domain, Observer* observer) = 0;

  virtual void RemoveObserver(PolicyDomain domain, Observer* observer) = 0;

  virtual const PolicyMap& GetPolicies(const PolicyNamespace& ns) const = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool IsInitializationComplete(PolicyDomain domain) const = 0;

  
  
  
  virtual void RefreshPolicies(const base::Closure& callback) = 0;
};

class POLICY_EXPORT PolicyChangeRegistrar : public PolicyService::Observer {
 public:
  typedef base::Callback<void(const Value*, const Value*)> UpdateCallback;

  
  
  
  
  PolicyChangeRegistrar(PolicyService* policy_service,
                        const PolicyNamespace& ns);

  virtual ~PolicyChangeRegistrar();

  
  
  
  
  void Observe(const std::string& policy_name, const UpdateCallback& callback);

  
  virtual void OnPolicyUpdated(const PolicyNamespace& ns,
                               const PolicyMap& previous,
                               const PolicyMap& current) OVERRIDE;

 private:
  typedef std::map<std::string, UpdateCallback> CallbackMap;

  PolicyService* policy_service_;
  PolicyNamespace ns_;
  CallbackMap callback_map_;

  DISALLOW_COPY_AND_ASSIGN(PolicyChangeRegistrar);
};

}  

#endif  
