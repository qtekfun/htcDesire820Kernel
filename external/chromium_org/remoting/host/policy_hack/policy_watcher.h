// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_POLICY_HACK_POLICY_WATCHER_H_
#define REMOTING_HOST_POLICY_HACK_POLICY_WATCHER_H_

#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/values.h"

namespace base {
class SingleThreadTaskRunner;
class TimeDelta;
class WaitableEvent;
}  

namespace remoting {
namespace policy_hack {

class PolicyWatcher {
 public:
  
  typedef base::Callback<void(scoped_ptr<base::DictionaryValue>)>
      PolicyCallback;

  explicit PolicyWatcher(
      scoped_refptr<base::SingleThreadTaskRunner> task_runner);
  virtual ~PolicyWatcher();

  
  
  
  
  virtual void StartWatching(const PolicyCallback& policy_callback);

  
  
  virtual void StopWatching(base::WaitableEvent* done);

  
  
  static PolicyWatcher* Create(
      scoped_refptr<base::SingleThreadTaskRunner> task_runner);

  
  static const char kNatPolicyName[];

  
  static const char kHostRequireTwoFactorPolicyName[];

  
  static const char kHostDomainPolicyName[];

  
  
  
  static const char kHostMatchUsernamePolicyName[];

  
  static const char kHostTalkGadgetPrefixPolicyName[];

  
  static const char kHostRequireCurtainPolicyName[];

  
  static const char kHostTokenUrlPolicyName[];
  static const char kHostTokenValidationUrlPolicyName[];

  
  static const char kHostAllowClientPairing[];

  
  static const char kHostDebugOverridePoliciesName[];

 protected:
  virtual void StartWatchingInternal() = 0;
  virtual void StopWatchingInternal() = 0;
  virtual void Reload() = 0;

  
  bool OnPolicyWatcherThread() const;

  
  
  void UpdatePolicies(const base::DictionaryValue* new_policy);

  
  
  void ScheduleFallbackReloadTask();
  void ScheduleReloadTask(const base::TimeDelta& delay);

  
  const base::DictionaryValue& Defaults() const;

 private:
  scoped_refptr<base::SingleThreadTaskRunner> task_runner_;

  PolicyCallback policy_callback_;

  scoped_ptr<base::DictionaryValue> old_policies_;
  scoped_ptr<base::DictionaryValue> default_values_;
  scoped_ptr<base::DictionaryValue> bad_type_values_;

  
  base::WeakPtrFactory<PolicyWatcher> weak_factory_;
};

}  
}  

#endif  
