// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_ASYNC_POLICY_PROVIDER_H_
#define COMPONENTS_POLICY_CORE_COMMON_ASYNC_POLICY_PROVIDER_H_

#include "base/cancelable_callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "components/policy/core/common/configuration_policy_provider.h"
#include "components/policy/policy_export.h"

namespace base {
class MessageLoopProxy;
}

namespace policy {

class AsyncPolicyLoader;
class PolicyBundle;
class SchemaRegistry;

class POLICY_EXPORT AsyncPolicyProvider : public ConfigurationPolicyProvider,
                                          public base::NonThreadSafe {
 public:
  
  
  
  AsyncPolicyProvider(SchemaRegistry* registry,
                      scoped_ptr<AsyncPolicyLoader> loader);
  virtual ~AsyncPolicyProvider();

  
  virtual void Init(SchemaRegistry* registry) OVERRIDE;
  virtual void Shutdown() OVERRIDE;
  virtual void RefreshPolicies() OVERRIDE;

 private:
  
  void ReloadAfterRefreshSync();

  
  void OnLoaderReloaded(scoped_ptr<PolicyBundle> bundle);

  
  
  
  
  static void LoaderUpdateCallback(scoped_refptr<base::MessageLoopProxy> loop,
                                   base::WeakPtr<AsyncPolicyProvider> weak_this,
                                   scoped_ptr<PolicyBundle> bundle);

  
  
  AsyncPolicyLoader* loader_;

  
  
  base::WeakPtrFactory<AsyncPolicyProvider> weak_factory_;

  
  
  base::CancelableClosure refresh_callback_;

  DISALLOW_COPY_AND_ASSIGN(AsyncPolicyProvider);
};

}  

#endif  
