// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_CONFIGURATION_POLICY_HANDLER_CHROMEOS_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_CONFIGURATION_POLICY_HANDLER_CHROMEOS_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/extensions/policy_handlers.h"
#include "chromeos/network/network_ui_data.h"
#include "components/onc/onc_constants.h"
#include "components/policy/core/browser/configuration_policy_handler.h"

namespace base {
class DictionaryValue;
class Value;
}

namespace policy {

class ExternalDataPolicyHandler : public TypeCheckingPolicyHandler {
 public:
  explicit ExternalDataPolicyHandler(const char* policy_name);
  virtual ~ExternalDataPolicyHandler();

  
  virtual bool CheckPolicySettings(const PolicyMap& policies,
                                   PolicyErrorMap* errors) OVERRIDE;

  virtual void ApplyPolicySettings(const PolicyMap& policies,
                                   PrefValueMap* prefs) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(ExternalDataPolicyHandler);
};

class NetworkConfigurationPolicyHandler : public TypeCheckingPolicyHandler {
 public:
  static NetworkConfigurationPolicyHandler* CreateForUserPolicy();
  static NetworkConfigurationPolicyHandler* CreateForDevicePolicy();

  virtual ~NetworkConfigurationPolicyHandler();

  
  virtual bool CheckPolicySettings(const PolicyMap& policies,
                                   PolicyErrorMap* errors) OVERRIDE;
  virtual void ApplyPolicySettings(const PolicyMap& policies,
                                   PrefValueMap* prefs) OVERRIDE;
  virtual void PrepareForDisplaying(PolicyMap* policies) const OVERRIDE;

 private:
  explicit NetworkConfigurationPolicyHandler(
      const char* policy_name,
      ::onc::ONCSource onc_source,
      const char* pref_path);

  
  
  
  
  static base::Value* SanitizeNetworkConfig(const base::Value* config);

  
  
  const ::onc::ONCSource onc_source_;

  
  const char* pref_path_;

  DISALLOW_COPY_AND_ASSIGN(NetworkConfigurationPolicyHandler);
};

class PinnedLauncherAppsPolicyHandler
    : public extensions::ExtensionListPolicyHandler {
 public:
  PinnedLauncherAppsPolicyHandler();
  virtual ~PinnedLauncherAppsPolicyHandler();

  
  virtual void ApplyPolicySettings(const PolicyMap& policies,
                                   PrefValueMap* prefs) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(PinnedLauncherAppsPolicyHandler);
};

class ScreenMagnifierPolicyHandler : public IntRangePolicyHandlerBase {
 public:
  ScreenMagnifierPolicyHandler();
  virtual ~ScreenMagnifierPolicyHandler();

  
  virtual void ApplyPolicySettings(const PolicyMap& policies,
                                   PrefValueMap* prefs) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(ScreenMagnifierPolicyHandler);
};

class LoginScreenPowerManagementPolicyHandler
    : public TypeCheckingPolicyHandler {
 public:
  LoginScreenPowerManagementPolicyHandler();
  virtual ~LoginScreenPowerManagementPolicyHandler();

  
  virtual bool CheckPolicySettings(const PolicyMap& policies,
                                   PolicyErrorMap* errors) OVERRIDE;
  virtual void ApplyPolicySettings(const PolicyMap& policies,
                                   PrefValueMap* prefs) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(LoginScreenPowerManagementPolicyHandler);
};

class DeprecatedIdleActionHandler : public IntRangePolicyHandlerBase {
 public:
  DeprecatedIdleActionHandler();
  virtual ~DeprecatedIdleActionHandler();

  
  virtual void ApplyPolicySettings(const PolicyMap& policies,
                                   PrefValueMap* prefs) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(DeprecatedIdleActionHandler);
};

}  

#endif  
