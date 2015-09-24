// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_POLICY_PROXY_POLICY_PROVIDER_H_
#define CHROME_BROWSER_CHROMEOS_POLICY_PROXY_POLICY_PROVIDER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "components/policy/core/common/configuration_policy_provider.h"

namespace policy {

class ProxyPolicyProvider : public ConfigurationPolicyProvider,
                            public ConfigurationPolicyProvider::Observer {
 public:
  ProxyPolicyProvider();
  virtual ~ProxyPolicyProvider();

  
  void SetDelegate(ConfigurationPolicyProvider* delegate);

  
  virtual void Shutdown() OVERRIDE;
  virtual void RefreshPolicies() OVERRIDE;

  
  virtual void OnUpdatePolicy(ConfigurationPolicyProvider* provider) OVERRIDE;

 private:
  ConfigurationPolicyProvider* delegate_;

  DISALLOW_COPY_AND_ASSIGN(ProxyPolicyProvider);
};

}  

#endif  
