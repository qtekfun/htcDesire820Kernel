// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_CONFIG_DIR_POLICY_PROVIDER_H_
#define CHROME_BROWSER_POLICY_CONFIG_DIR_POLICY_PROVIDER_H_
#pragma once

#include "chrome/browser/policy/file_based_policy_provider.h"

namespace policy {

class ConfigDirPolicyProvider : public FileBasedPolicyProvider {
 public:
  ConfigDirPolicyProvider(
      const ConfigurationPolicyProvider::PolicyDefinitionList* policy_list,
      const FilePath& config_dir);

 private:
  DISALLOW_COPY_AND_ASSIGN(ConfigDirPolicyProvider);
};

class ConfigDirPolicyProviderDelegate
    : public FileBasedPolicyProvider::ProviderDelegate {
 public:
  explicit ConfigDirPolicyProviderDelegate(const FilePath& config_dir);

  
  virtual DictionaryValue* Load();
  virtual base::Time GetLastModification();

 private:
  DISALLOW_COPY_AND_ASSIGN(ConfigDirPolicyProviderDelegate);
};

}  

#endif  
