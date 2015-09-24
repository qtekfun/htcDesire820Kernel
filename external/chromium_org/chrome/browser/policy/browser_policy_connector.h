// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_BROWSER_POLICY_CONNECTOR_H_
#define CHROME_BROWSER_POLICY_BROWSER_POLICY_CONNECTOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "components/policy/core/browser/configuration_policy_handler_list.h"
#include "components/policy/core/common/cloud/cloud_policy_constants.h"
#include "components/policy/core/common/schema.h"
#include "components/policy/core/common/schema_registry.h"

#if defined(OS_CHROMEOS)
#include "chrome/browser/chromeos/policy/proxy_policy_provider.h"
#endif

class PrefRegistrySimple;
class PrefService;

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class ConfigurationPolicyProvider;
class DeviceManagementService;
class PolicyService;
class PolicyStatisticsCollector;

#if defined(OS_CHROMEOS)
class AppPackUpdater;
class DeviceCloudPolicyManagerChromeOS;
class DeviceLocalAccountPolicyService;
class EnterpriseInstallAttributes;
class NetworkConfigurationUpdater;
#endif

class BrowserPolicyConnector {
 public:
  
  
  BrowserPolicyConnector();

  
  virtual ~BrowserPolicyConnector();

  
  
  void Init(PrefService* local_state,
            scoped_refptr<net::URLRequestContextGetter> request_context);

  
  
  
  
  void Shutdown();

  
  bool is_initialized() const { return is_initialized_; }

  
  const Schema& GetChromeSchema() const;

  
  
  
  CombinedSchemaRegistry* GetSchemaRegistry();

  
  ConfigurationPolicyProvider* GetPlatformProvider();

  
  
  PolicyService* GetPolicyService();

#if defined(OS_CHROMEOS)
  
  
  bool IsEnterpriseManaged();

  
  std::string GetEnterpriseDomain();

  
  
  
  
  DeviceMode GetDeviceMode();
#endif

  
  
  void ScheduleServiceInitialization(int64 delay_milliseconds);

  const ConfigurationPolicyHandlerList* GetHandlerList() const;

  
  
  UserAffiliation GetUserAffiliation(const std::string& user_name);

  DeviceManagementService* device_management_service() {
    return device_management_service_.get();
  }

#if defined(OS_CHROMEOS)
  AppPackUpdater* GetAppPackUpdater();

  DeviceCloudPolicyManagerChromeOS* GetDeviceCloudPolicyManager() {
    return device_cloud_policy_manager_.get();
  }
  DeviceLocalAccountPolicyService* GetDeviceLocalAccountPolicyService() {
    return device_local_account_policy_service_.get();
  }
  EnterpriseInstallAttributes* GetInstallAttributes() {
    return install_attributes_.get();
  }

  
  
  
  
  
  
  
  
  
  void SetUserPolicyDelegate(ConfigurationPolicyProvider* user_policy_provider);

  
  
  static void SetInstallAttributesForTesting(
      EnterpriseInstallAttributes* attributes);
#endif

  
  
  
  
  
  
  static void SetPolicyProviderForTesting(
      ConfigurationPolicyProvider* provider);

  
  
  
  static bool IsNonEnterpriseUser(const std::string& username);

  
  static void RegisterPrefs(PrefRegistrySimple* registry);

 private:
  
  void SetTimezoneIfPolicyAvailable();

  ConfigurationPolicyProvider* CreatePlatformProvider();

  
  bool is_initialized_;

  PrefService* local_state_;
  scoped_refptr<net::URLRequestContextGetter> request_context_;

  
  
  
  
  scoped_ptr<ConfigurationPolicyHandlerList> handler_list_;

  
  
  Schema chrome_schema_;

  
  CombinedSchemaRegistry schema_registry_;

  scoped_ptr<ConfigurationPolicyProvider> platform_provider_;

  
#if defined(OS_CHROMEOS)
  scoped_ptr<EnterpriseInstallAttributes> install_attributes_;
  scoped_ptr<DeviceCloudPolicyManagerChromeOS> device_cloud_policy_manager_;
  scoped_ptr<DeviceLocalAccountPolicyService>
      device_local_account_policy_service_;

  
  
  
  
  ProxyPolicyProvider global_user_cloud_policy_provider_;
#endif

  
  scoped_ptr<PolicyService> policy_service_;

  scoped_ptr<PolicyStatisticsCollector> policy_statistics_collector_;

  scoped_ptr<DeviceManagementService> device_management_service_;

  
  
  base::WeakPtrFactory<BrowserPolicyConnector> weak_ptr_factory_;

#if defined(OS_CHROMEOS)
  scoped_ptr<AppPackUpdater> app_pack_updater_;
  scoped_ptr<NetworkConfigurationUpdater> network_configuration_updater_;
#endif

  DISALLOW_COPY_AND_ASSIGN(BrowserPolicyConnector);
};

}  

#endif  
