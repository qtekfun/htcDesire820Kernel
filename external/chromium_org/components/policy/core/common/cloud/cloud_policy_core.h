// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_CORE_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_CORE_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/pref_member.h"
#include "components/policy/core/common/cloud/cloud_policy_constants.h"
#include "components/policy/policy_export.h"

class PrefService;

namespace base {
class SequencedTaskRunner;
}

namespace policy {

class CloudPolicyClient;
class CloudPolicyRefreshScheduler;
class CloudPolicyService;
class CloudPolicyStore;

class POLICY_EXPORT CloudPolicyCore {
 public:
  
  class POLICY_EXPORT Observer {
   public:
    virtual ~Observer();

    
    virtual void OnCoreConnected(CloudPolicyCore* core) = 0;

    
    virtual void OnRefreshSchedulerStarted(CloudPolicyCore* core) = 0;

    
    virtual void OnCoreDisconnecting(CloudPolicyCore* core) = 0;
  };

  
  CloudPolicyCore(const PolicyNamespaceKey& policy_ns_key,
                  CloudPolicyStore* store,
                  const scoped_refptr<base::SequencedTaskRunner>& task_runner);
  ~CloudPolicyCore();

  CloudPolicyClient* client() { return client_.get(); }
  const CloudPolicyClient* client() const { return client_.get(); }

  CloudPolicyStore* store() { return store_; }
  const CloudPolicyStore* store() const { return store_; }

  CloudPolicyService* service() { return service_.get(); }
  const CloudPolicyService* service() const { return service_.get(); }

  CloudPolicyRefreshScheduler* refresh_scheduler() {
    return refresh_scheduler_.get();
  }
  const CloudPolicyRefreshScheduler* refresh_scheduler() const {
    return refresh_scheduler_.get();
  }

  
  void Connect(scoped_ptr<CloudPolicyClient> client);

  
  void Disconnect();

  
  
  void RefreshSoon();

  
  void StartRefreshScheduler();

  
  
  void TrackRefreshDelayPref(PrefService* pref_service,
                             const std::string& refresh_pref_name);

  
  void AddObserver(Observer* observer);

  
  void RemoveObserver(Observer* observer);

 private:
  
  void UpdateRefreshDelayFromPref();

  PolicyNamespaceKey policy_ns_key_;
  CloudPolicyStore* store_;
  scoped_refptr<base::SequencedTaskRunner> task_runner_;
  scoped_ptr<CloudPolicyClient> client_;
  scoped_ptr<CloudPolicyService> service_;
  scoped_ptr<CloudPolicyRefreshScheduler> refresh_scheduler_;
  scoped_ptr<IntegerPrefMember> refresh_delay_;
  ObserverList<Observer, true> observers_;

  DISALLOW_COPY_AND_ASSIGN(CloudPolicyCore);
};

}  

#endif  
