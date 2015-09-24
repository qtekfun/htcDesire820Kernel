// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_CLOUD_POLICY_SUBSYSTEM_H_
#define CHROME_BROWSER_POLICY_CLOUD_POLICY_SUBSYSTEM_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/prefs/pref_member.h"
#include "content/common/notification_observer.h"
#include "net/base/network_change_notifier.h"

class PrefService;

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class CloudPolicyCacheBase;
class CloudPolicyController;
class CloudPolicyIdentityStrategy;
class ConfigurationPolicyProvider;
class DeviceManagementService;
class DeviceTokenFetcher;
class PolicyNotifier;

class CloudPolicySubsystem
    : public NotificationObserver,
      public net::NetworkChangeNotifier::IPAddressObserver {
 public:
  enum PolicySubsystemState {
    UNENROLLED,     
    BAD_GAIA_TOKEN, 
    UNMANAGED,      
    NETWORK_ERROR,  
    LOCAL_ERROR,    
    TOKEN_FETCHED,  
    SUCCESS         
  };

  enum ErrorDetails {
    NO_DETAILS,            
    DMTOKEN_NETWORK_ERROR, 
    POLICY_NETWORK_ERROR,  
    BAD_DMTOKEN,           
    POLICY_LOCAL_ERROR,    
    SIGNATURE_MISMATCH,    
  };

  class Observer {
   public:
    virtual ~Observer() {}
    virtual void OnPolicyStateChanged(PolicySubsystemState state,
                                      ErrorDetails error_details) = 0;
  };

  class ObserverRegistrar {
   public:
    ObserverRegistrar(CloudPolicySubsystem* cloud_policy_subsystem,
                      CloudPolicySubsystem::Observer* observer);
    ~ObserverRegistrar();

   private:
    PolicyNotifier* policy_notifier_;
    CloudPolicySubsystem::Observer* observer_;
    DISALLOW_COPY_AND_ASSIGN(ObserverRegistrar);
  };

  CloudPolicySubsystem(CloudPolicyIdentityStrategy* identity_strategy,
                       CloudPolicyCacheBase* policy_cache);
  virtual ~CloudPolicySubsystem();

  
  virtual void OnIPAddressChanged() OVERRIDE;

  
  void Initialize(PrefService* prefs,
                  net::URLRequestContextGetter* request_context);

  
  
  void Shutdown();

  
  PolicySubsystemState state();
  ErrorDetails error_details();

  
  
  void StopAutoRetry();

  ConfigurationPolicyProvider* GetManagedPolicyProvider();
  ConfigurationPolicyProvider* GetRecommendedPolicyProvider();

  
  static void RegisterPrefs(PrefService* pref_service);

 private:
  
  void UpdatePolicyRefreshRate();

  
  PolicyNotifier* notifier() {
    return notifier_.get();
  }

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  PrefService* prefs_;

  
  IntegerPrefMember policy_refresh_rate_;

  
  scoped_ptr<PolicyNotifier> notifier_;
  scoped_ptr<DeviceManagementService> device_management_service_;
  scoped_ptr<DeviceTokenFetcher> device_token_fetcher_;
  scoped_ptr<CloudPolicyCacheBase> cloud_policy_cache_;
  scoped_ptr<CloudPolicyController> cloud_policy_controller_;

  DISALLOW_COPY_AND_ASSIGN(CloudPolicySubsystem);
};

}  

#endif  
