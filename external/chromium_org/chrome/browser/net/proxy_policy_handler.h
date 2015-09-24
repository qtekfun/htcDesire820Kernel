// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NET_PROXY_POLICY_HANDLER_H_
#define CHROME_BROWSER_NET_PROXY_POLICY_HANDLER_H_

#include <string>

#include "base/basictypes.h"
#include "components/policy/core/browser/configuration_policy_handler.h"

namespace policy {

class ProxyMap;
class ProxyErrorMap;

class ProxyPolicyHandler : public ConfigurationPolicyHandler {
 public:
  
  
  
  
  
  enum ProxyModeType {
    
    PROXY_SERVER_MODE = 0,
    
    PROXY_AUTO_DETECT_PROXY_SERVER_MODE = 1,
    
    PROXY_MANUALLY_CONFIGURED_PROXY_SERVER_MODE = 2,
    
    PROXY_USE_SYSTEM_PROXY_SERVER_MODE = 3,

    MODE_COUNT
  };

  ProxyPolicyHandler();
  virtual ~ProxyPolicyHandler();

  
  virtual bool CheckPolicySettings(const PolicyMap& policies,
                                   PolicyErrorMap* errors) OVERRIDE;
  virtual void ApplyPolicySettings(const PolicyMap& policies,
                                   PrefValueMap* prefs) OVERRIDE;

 private:
  const base::Value* GetProxyPolicyValue(const PolicyMap& policies,
                                         const char* policy_name);

  
  
  
  bool CheckProxyModeAndServerMode(const PolicyMap& policies,
                                   PolicyErrorMap* errors,
                                   std::string* mode_value);

  DISALLOW_COPY_AND_ASSIGN(ProxyPolicyHandler);
};

}  

#endif  
