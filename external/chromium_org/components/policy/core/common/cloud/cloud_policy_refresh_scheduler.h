// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_REFRESH_SCHEDULER_H_
#define COMPONENTS_POLICY_CORE_COMMON_CLOUD_CLOUD_POLICY_REFRESH_SCHEDULER_H_

#include "base/basictypes.h"
#include "base/cancelable_callback.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "components/policy/core/common/cloud/cloud_policy_client.h"
#include "components/policy/core/common/cloud/cloud_policy_store.h"
#include "components/policy/core/common/cloud/rate_limiter.h"
#include "components/policy/policy_export.h"
#include "net/base/network_change_notifier.h"

namespace base {
class SequencedTaskRunner;
}

namespace policy {

class POLICY_EXPORT CloudPolicyRefreshScheduler
    : public CloudPolicyClient::Observer,
      public CloudPolicyStore::Observer,
      public net::NetworkChangeNotifier::IPAddressObserver {
 public:
  
  static const int64 kDefaultRefreshDelayMs;
  static const int64 kUnmanagedRefreshDelayMs;
  static const int64 kWithInvalidationsRefreshDelayMs;
  static const int64 kInitialErrorRetryDelayMs;

  
  static const int64 kRefreshDelayMinMs;
  static const int64 kRefreshDelayMaxMs;

  
  
  CloudPolicyRefreshScheduler(
      CloudPolicyClient* client,
      CloudPolicyStore* store,
      const scoped_refptr<base::SequencedTaskRunner>& task_runner);
  virtual ~CloudPolicyRefreshScheduler();

  base::Time last_refresh() const { return last_refresh_; }
  int64 refresh_delay() const { return refresh_delay_ms_; }

  
  void SetRefreshDelay(int64 refresh_delay);

  
  
  void RefreshSoon();

  
  
  
  
  
  void SetInvalidationServiceAvailability(bool is_available);

  
  
  bool invalidations_available() {
    return invalidations_available_;
  }

  
  virtual void OnPolicyFetched(CloudPolicyClient* client) OVERRIDE;
  virtual void OnRegistrationStateChanged(CloudPolicyClient* client) OVERRIDE;
  virtual void OnClientError(CloudPolicyClient* client) OVERRIDE;

  
  virtual void OnStoreLoaded(CloudPolicyStore* store) OVERRIDE;
  virtual void OnStoreError(CloudPolicyStore* store) OVERRIDE;

  
  virtual void OnIPAddressChanged() OVERRIDE;

 private:
  
  
  
  
  
  void UpdateLastRefreshFromPolicy();

  
  void RefreshNow();

  
  
  void ScheduleRefresh();

  
  void PerformRefresh();

  
  
  void RefreshAfter(int delta_ms);

  
  void WaitForInvalidationService();

  
  void OnWaitForInvalidationServiceTimeout();

  
  
  bool WaitingForInvalidationService() const;

  CloudPolicyClient* client_;
  CloudPolicyStore* store_;

  
  const scoped_refptr<base::SequencedTaskRunner> task_runner_;

  
  base::CancelableClosure refresh_callback_;

  
  base::Time last_refresh_;

  
  int64 error_retry_delay_ms_;

  
  int64 refresh_delay_ms_;

  
  RateLimiter rate_limiter_;

  
  
  bool invalidations_available_;

  
  
  
  
  base::CancelableClosure wait_for_invalidations_timeout_callback_;

  
  
  base::Time creation_time_;

  DISALLOW_COPY_AND_ASSIGN(CloudPolicyRefreshScheduler);
};

}  

#endif  
