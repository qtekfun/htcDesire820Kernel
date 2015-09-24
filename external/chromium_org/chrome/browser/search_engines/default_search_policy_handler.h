// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SEARCH_ENGINES_DEFAULT_SEARCH_POLICY_HANDLER_H_
#define CHROME_BROWSER_SEARCH_ENGINES_DEFAULT_SEARCH_POLICY_HANDLER_H_

#include <vector>

#include "components/policy/core/browser/configuration_policy_handler.h"

namespace policy {

class DefaultSearchEncodingsPolicyHandler
    : public TypeCheckingPolicyHandler {
 public:
  DefaultSearchEncodingsPolicyHandler();
  virtual ~DefaultSearchEncodingsPolicyHandler();

  
  virtual void ApplyPolicySettings(const PolicyMap& policies,
                                   PrefValueMap* prefs) OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(DefaultSearchEncodingsPolicyHandler);
};

class DefaultSearchPolicyHandler : public ConfigurationPolicyHandler {
 public:
  DefaultSearchPolicyHandler();
  virtual ~DefaultSearchPolicyHandler();

  
  virtual bool CheckPolicySettings(const PolicyMap& policies,
                                   PolicyErrorMap* errors) OVERRIDE;
  virtual void ApplyPolicySettings(const PolicyMap& policies,
                                   PrefValueMap* prefs) OVERRIDE;

 private:
  
  
  bool CheckIndividualPolicies(const PolicyMap& policies,
                               PolicyErrorMap* errors);

  
  bool HasDefaultSearchPolicy(const PolicyMap& policies,
                              const char* policy_name);

  
  bool AnyDefaultSearchPoliciesSpecified(const PolicyMap& policies);

  
  bool DefaultSearchProviderIsDisabled(const PolicyMap& policies);

  
  
  bool DefaultSearchURLIsValid(const PolicyMap& policies,
                               const Value** url_value,
                               std::string* url_string);

  
  
  void EnsureStringPrefExists(PrefValueMap* prefs, const std::string& path);

  
  
  void EnsureListPrefExists(PrefValueMap* prefs, const std::string& path);

  
  std::vector<TypeCheckingPolicyHandler*> handlers_;

  DISALLOW_COPY_AND_ASSIGN(DefaultSearchPolicyHandler);
};

}  

#endif  
