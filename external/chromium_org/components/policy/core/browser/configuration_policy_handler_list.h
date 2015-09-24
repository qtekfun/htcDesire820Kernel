// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_BROWSER_CONFIGURATION_POLICY_HANDLER_LIST_H_
#define COMPONENTS_POLICY_CORE_BROWSER_CONFIGURATION_POLICY_HANDLER_LIST_H_

#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "components/policy/core/common/policy_details.h"
#include "components/policy/policy_export.h"

class PrefValueMap;

namespace policy {

class ConfigurationPolicyHandler;
class PolicyErrorMap;
class PolicyMap;
struct PolicyToPreferenceMapEntry;

class POLICY_EXPORT ConfigurationPolicyHandlerList {
 public:
  explicit ConfigurationPolicyHandlerList(
      const GetChromePolicyDetailsCallback& details_callback);
  ~ConfigurationPolicyHandlerList();

  
  void AddHandler(scoped_ptr<ConfigurationPolicyHandler> handler);

  
  
  
  void ApplyPolicySettings(const PolicyMap& policies,
                           PrefValueMap* prefs,
                           PolicyErrorMap* errors) const;

  
  void PrepareForDisplaying(PolicyMap* policies) const;

 private:
  std::vector<ConfigurationPolicyHandler*> handlers_;
  GetChromePolicyDetailsCallback details_callback_;

  DISALLOW_COPY_AND_ASSIGN(ConfigurationPolicyHandlerList);
};

}  

#endif  
