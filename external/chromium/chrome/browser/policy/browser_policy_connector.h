// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_BROWSER_POLICY_CONNECTOR_H_
#define CHROME_BROWSER_POLICY_BROWSER_POLICY_CONNECTOR_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/task.h"
#include "chrome/browser/policy/enterprise_install_attributes.h"

class PrefService;
class TestingBrowserProcess;
class TokenService;

namespace net {
class URLRequestContextGetter;
}

namespace policy {

class CloudPolicySubsystem;
class ConfigurationPolicyProvider;
class DevicePolicyIdentityStrategy;

class BrowserPolicyConnector {
 public:
  BrowserPolicyConnector();
  ~BrowserPolicyConnector();

  ConfigurationPolicyProvider* GetManagedPlatformProvider() const;
  ConfigurationPolicyProvider* GetManagedCloudProvider() const;
  ConfigurationPolicyProvider* GetRecommendedPlatformProvider() const;
  ConfigurationPolicyProvider* GetRecommendedCloudProvider() const;

  
  
  
  CloudPolicySubsystem* cloud_policy_subsystem() {
    return cloud_policy_subsystem_.get();
  }

  
  
  void SetCredentials(const std::string& owner_email,
                      const std::string& gaia_token);

  
  
  bool IsEnterpriseManaged();

  
  EnterpriseInstallAttributes::LockResult LockDevice(const std::string& user);

  
  std::string GetEnterpriseDomain();

  
  
  
  void StopAutoRetry();

  
  void FetchPolicy();

 private:
  friend class ::TestingBrowserProcess;

  static ConfigurationPolicyProvider* CreateManagedPlatformProvider();
  static ConfigurationPolicyProvider* CreateRecommendedPlatformProvider();

  
  
  BrowserPolicyConnector(
      ConfigurationPolicyProvider* managed_platform_provider,
      ConfigurationPolicyProvider* recommended_platform_provider);

  
  void Initialize();

  scoped_ptr<ConfigurationPolicyProvider> managed_platform_provider_;
  scoped_ptr<ConfigurationPolicyProvider> recommended_platform_provider_;

#if defined(OS_CHROMEOS)
  scoped_ptr<DevicePolicyIdentityStrategy> identity_strategy_;
  scoped_ptr<EnterpriseInstallAttributes> install_attributes_;
#endif
  scoped_ptr<CloudPolicySubsystem> cloud_policy_subsystem_;

  ScopedRunnableMethodFactory<BrowserPolicyConnector> method_factory_;

  DISALLOW_COPY_AND_ASSIGN(BrowserPolicyConnector);
};

}  

#endif  
