// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_ASYNCHRONOUS_POLICY_PROVIDER_H_
#define CHROME_BROWSER_POLICY_ASYNCHRONOUS_POLICY_PROVIDER_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/policy/configuration_policy_provider.h"

namespace policy {

class AsynchronousPolicyLoader;

class AsynchronousPolicyProvider
    : public ConfigurationPolicyProvider,
      public base::NonThreadSafe {
 public:
  
  
  class Delegate {
   public:
    virtual ~Delegate() {}

    virtual DictionaryValue* Load() = 0;
  };

  
  AsynchronousPolicyProvider(
      const PolicyDefinitionList* policy_list,
      scoped_refptr<AsynchronousPolicyLoader> loader);
  virtual ~AsynchronousPolicyProvider();

  
  virtual bool Provide(ConfigurationPolicyStoreInterface* store);

  
  scoped_refptr<AsynchronousPolicyLoader> loader();

 protected:
  
  scoped_refptr<AsynchronousPolicyLoader> loader_;

 private:
  
  virtual void AddObserver(ConfigurationPolicyProvider::Observer* observer);
  virtual void RemoveObserver(ConfigurationPolicyProvider::Observer* observer);

  DISALLOW_COPY_AND_ASSIGN(AsynchronousPolicyProvider);
};

}  

#endif  
