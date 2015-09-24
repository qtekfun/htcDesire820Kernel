// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_POLICY_ASYNCHRONOUS_POLICY_LOADER_H_
#define CHROME_BROWSER_POLICY_ASYNCHRONOUS_POLICY_LOADER_H_
#pragma once

#include "base/memory/ref_counted.h"
#include "base/message_loop.h"
#include "base/observer_list.h"
#include "base/values.h"
#include "chrome/browser/policy/asynchronous_policy_provider.h"
#include "chrome/browser/policy/configuration_policy_provider.h"

namespace policy {

class AsynchronousPolicyLoader
    : public base::RefCountedThreadSafe<AsynchronousPolicyLoader> {
 public:
  explicit AsynchronousPolicyLoader(
      AsynchronousPolicyProvider::Delegate* delegate,
      int reload_interval_minutes);

  
  virtual void Init();

  
  
  virtual void Reload();

  
  virtual void Stop();

  void AddObserver(ConfigurationPolicyProvider::Observer* observer);
  void RemoveObserver(ConfigurationPolicyProvider::Observer* observer);

  const DictionaryValue* policy() const { return policy_.get(); }

 protected:
  friend class UpdatePolicyTask;

  
  
  friend class base::RefCountedThreadSafe<AsynchronousPolicyLoader>;
  virtual ~AsynchronousPolicyLoader();

  
  
  void PostUpdatePolicyTask(DictionaryValue* new_policy);

  AsynchronousPolicyProvider::Delegate* delegate() {
    return delegate_.get();
  }

  
  virtual void InitOnFileThread();

  
  virtual void StopOnFileThread();

  
  
  void ScheduleReloadTask(const base::TimeDelta& delay);

  
  
  void ScheduleFallbackReloadTask();

  void CancelReloadTask();

  
  void ReloadFromTask();

 private:
  friend class AsynchronousPolicyLoaderTest;

  
  
  void InitAfterFileThreadAvailable();

  
  
  
  
  void UpdatePolicy(DictionaryValue* new_policy);

  
  scoped_ptr<AsynchronousPolicyProvider::Delegate> delegate_;

  
  scoped_ptr<DictionaryValue> policy_;

  
  
  CancelableTask* reload_task_;

  
  const base::TimeDelta  reload_interval_;

  
  
  
  MessageLoop* origin_loop_;

  
  bool stopped_;

  ObserverList<ConfigurationPolicyProvider::Observer, true> observer_list_;

  DISALLOW_COPY_AND_ASSIGN(AsynchronousPolicyLoader);
};

}  

#endif  
