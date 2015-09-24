// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_CONFIGURATION_POLICY_PROVIDER_WIN_H_
#define CHROME_BROWSER_POLICY_CONFIGURATION_POLICY_PROVIDER_WIN_H_
#pragma once

#include "chrome/browser/policy/asynchronous_policy_provider.h"

namespace policy {

class ConfigurationPolicyProviderWin : public AsynchronousPolicyProvider {
 public:
  explicit ConfigurationPolicyProviderWin(
      const PolicyDefinitionList* policy_list);
  virtual ~ConfigurationPolicyProviderWin() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(ConfigurationPolicyProviderWin);
};

}  

#endif  
