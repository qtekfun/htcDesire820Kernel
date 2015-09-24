// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_POLICY_HANDLERS_H_
#define CHROME_BROWSER_EXTENSIONS_POLICY_HANDLERS_H_

#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "components/policy/core/browser/configuration_policy_handler.h"

namespace policy {
class PolicyMap;
class PolicyErrorMap;
}  

namespace extensions {

class ExtensionListPolicyHandler
    : public policy::TypeCheckingPolicyHandler {
 public:
  ExtensionListPolicyHandler(const char* policy_name,
                             const char* pref_path,
                             bool allow_wildcards);
  virtual ~ExtensionListPolicyHandler();

  
  virtual bool CheckPolicySettings(const policy::PolicyMap& policies,
                                   policy::PolicyErrorMap* errors) OVERRIDE;
  virtual void ApplyPolicySettings(const policy::PolicyMap& policies,
                                   PrefValueMap* prefs) OVERRIDE;

 protected:
  const char* pref_path() const;

  
  bool CheckAndGetList(const policy::PolicyMap& policies,
                       policy::PolicyErrorMap* errors,
                       scoped_ptr<base::ListValue>* extension_ids);

 private:
  const char* pref_path_;
  bool allow_wildcards_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionListPolicyHandler);
};

class ExtensionInstallForcelistPolicyHandler
    : public policy::TypeCheckingPolicyHandler {
 public:
  ExtensionInstallForcelistPolicyHandler();
  virtual ~ExtensionInstallForcelistPolicyHandler();

  
  virtual bool CheckPolicySettings(const policy::PolicyMap& policies,
                                   policy::PolicyErrorMap* errors) OVERRIDE;
  virtual void ApplyPolicySettings(const policy::PolicyMap& policies,
                                   PrefValueMap* prefs) OVERRIDE;

 private:
  
  bool ParseList(const base::Value* policy_value,
                 base::DictionaryValue* extension_dict,
                 policy::PolicyErrorMap* errors);

  DISALLOW_COPY_AND_ASSIGN(ExtensionInstallForcelistPolicyHandler);
};

class ExtensionURLPatternListPolicyHandler
    : public policy::TypeCheckingPolicyHandler {
 public:
  ExtensionURLPatternListPolicyHandler(const char* policy_name,
                                       const char* pref_path);
  virtual ~ExtensionURLPatternListPolicyHandler();

  
  virtual bool CheckPolicySettings(const policy::PolicyMap& policies,
                                   policy::PolicyErrorMap* errors) OVERRIDE;
  virtual void ApplyPolicySettings(const policy::PolicyMap& policies,
                                   PrefValueMap* prefs) OVERRIDE;

 private:
  const char* pref_path_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionURLPatternListPolicyHandler);
};

}  

#endif  
