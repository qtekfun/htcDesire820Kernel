// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_CLOUD_EXTERNAL_DATA_POLICY_OBSERVER_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_CLOUD_EXTERNAL_DATA_POLICY_OBSERVER_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/policy/device_local_account_policy_service.h"
#include "chrome/browser/chromeos/settings/cros_settings.h"
#include "components/policy/core/common/policy_map.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace chromeos {
class UserManager;
}

namespace policy {

class CloudExternalDataPolicyObserver
    : public content::NotificationObserver,
      public DeviceLocalAccountPolicyService::Observer {
 public:
  class Delegate {
   public:
    
    virtual void OnExternalDataSet(const std::string& policy,
                                   const std::string& user_id);

    
    virtual void OnExternalDataCleared(const std::string& policy,
                                       const std::string& user_id);

    
    
    
    
    
    virtual void OnExternalDataFetched(const std::string& policy,
                                       const std::string& user_id,
                                       scoped_ptr<std::string> data);

   protected:
    virtual ~Delegate();
  };

  CloudExternalDataPolicyObserver(
      chromeos::CrosSettings* cros_settings,
      chromeos::UserManager* user_manager,
      DeviceLocalAccountPolicyService* device_local_account_policy_service,
      const std::string& policy,
      Delegate* delegate);
  virtual ~CloudExternalDataPolicyObserver();

  void Init();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnPolicyUpdated(const std::string& user_id) OVERRIDE;
  virtual void OnDeviceLocalAccountsChanged() OVERRIDE;

 private:
  
  class PolicyServiceObserver;

  void RetrieveDeviceLocalAccounts();

  
  
  
  
  void HandleExternalDataPolicyUpdate(const std::string& user_id,
                                      const PolicyMap::Entry* entry);

  void OnExternalDataFetched(const std::string& user_id,
                             scoped_ptr<std::string> data);

  
  
  typedef std::map<std::string, PolicyMap::Entry> DeviceLocalAccountEntryMap;
  DeviceLocalAccountEntryMap device_local_account_entries_;

  
  
  typedef std::map<std::string, linked_ptr<PolicyServiceObserver> >
      LoggedInUserObserverMap;
  LoggedInUserObserverMap logged_in_user_observers_;

  chromeos::CrosSettings* cros_settings_;
  chromeos::UserManager* user_manager_;
  DeviceLocalAccountPolicyService* device_local_account_policy_service_;

  
  std::string policy_;

  Delegate* delegate_;

  content::NotificationRegistrar notification_registrar_;
  scoped_ptr<chromeos::CrosSettings::ObserverSubscription>
      device_local_accounts_subscription_;

  
  
  
  typedef base::WeakPtrFactory<CloudExternalDataPolicyObserver>
      WeakPtrFactory;
  typedef std::map<std::string, linked_ptr<WeakPtrFactory> > FetchWeakPtrMap;
  FetchWeakPtrMap fetch_weak_ptrs_;

  base::WeakPtrFactory<CloudExternalDataPolicyObserver> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(CloudExternalDataPolicyObserver);
};

}  

#endif  
