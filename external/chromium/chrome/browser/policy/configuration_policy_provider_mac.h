// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_CONFIGURATION_POLICY_PROVIDER_MAC_H_
#define CHROME_BROWSER_POLICY_CONFIGURATION_POLICY_PROVIDER_MAC_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/policy/configuration_policy_store_interface.h"
#include "chrome/browser/policy/file_based_policy_provider.h"
#include "chrome/browser/preferences_mac.h"

namespace policy {

class MacPreferencesPolicyProviderDelegate
    : public FileBasedPolicyProvider::ProviderDelegate {
 public:
  
  MacPreferencesPolicyProviderDelegate(
      MacPreferences* preferences,
      const ConfigurationPolicyProvider::PolicyDefinitionList* policy_list);
  virtual ~MacPreferencesPolicyProviderDelegate();

  
  virtual DictionaryValue* Load();
  virtual base::Time GetLastModification();

 private:
  
  
  
  
  
  const ConfigurationPolicyProvider::PolicyDefinitionList* policy_list_;

  scoped_ptr<MacPreferences> preferences_;

  DISALLOW_COPY_AND_ASSIGN(MacPreferencesPolicyProviderDelegate);
};

class ConfigurationPolicyProviderMac
    : public FileBasedPolicyProvider {
 public:
  explicit ConfigurationPolicyProviderMac(
      const ConfigurationPolicyProvider::PolicyDefinitionList* policy_list);
  
  ConfigurationPolicyProviderMac(
      const ConfigurationPolicyProvider::PolicyDefinitionList* policy_list,
      MacPreferences* preferences);

  DISALLOW_COPY_AND_ASSIGN(ConfigurationPolicyProviderMac);
};

}  

#endif  
