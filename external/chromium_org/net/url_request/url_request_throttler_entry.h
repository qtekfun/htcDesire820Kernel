// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_THROTTLER_ENTRY_H_
#define NET_URL_REQUEST_URL_REQUEST_THROTTLER_ENTRY_H_

#include <queue>
#include <string>

#include "base/basictypes.h"
#include "base/time/time.h"
#include "net/base/backoff_entry.h"
#include "net/base/net_log.h"
#include "net/url_request/url_request_throttler_entry_interface.h"

namespace net {

class URLRequestThrottlerManager;

class NET_EXPORT URLRequestThrottlerEntry
    : public URLRequestThrottlerEntryInterface {
 public:
  
  static const int kDefaultSlidingWindowPeriodMs;

  
  static const int kDefaultMaxSendThreshold;

  
  static const int kDefaultNumErrorsToIgnore;

  
  static const int kDefaultInitialDelayMs;

  
  static const double kDefaultMultiplyFactor;

  
  
  static const double kDefaultJitterFactor;

  
  static const int kDefaultMaximumBackoffMs;

  
  static const int kDefaultEntryLifetimeMs;

  
  
  static const char kExponentialThrottlingHeader[];

  
  
  static const char kExponentialThrottlingDisableValue[];

  
  URLRequestThrottlerEntry(URLRequestThrottlerManager* manager,
                           const std::string& url_id);

  
  
  
  URLRequestThrottlerEntry(URLRequestThrottlerManager* manager,
                           const std::string& url_id,
                           int sliding_window_period_ms,
                           int max_send_threshold,
                           int initial_backoff_ms,
                           double multiply_factor,
                           double jitter_factor,
                           int maximum_backoff_ms);

  
  
  bool IsEntryOutdated() const;

  
  void DisableBackoffThrottling();

  
  void DetachManager();

  
  virtual bool ShouldRejectRequest(const URLRequest& request) const OVERRIDE;
  virtual int64 ReserveSendingTimeForNextRequest(
      const base::TimeTicks& earliest_time) OVERRIDE;
  virtual base::TimeTicks GetExponentialBackoffReleaseTime() const OVERRIDE;
  virtual void UpdateWithResponse(
      const std::string& host,
      const URLRequestThrottlerHeaderInterface* response) OVERRIDE;
  virtual void ReceivedContentWasMalformed(int response_code) OVERRIDE;

 protected:
  virtual ~URLRequestThrottlerEntry();

  void Initialize();

  
  
  bool IsConsideredError(int response_code);

  
  virtual base::TimeTicks ImplGetTimeNow() const;

  
  void HandleThrottlingHeader(const std::string& header_value,
                              const std::string& host);

  
  
  virtual const BackoffEntry* GetBackoffEntry() const;
  virtual BackoffEntry* GetBackoffEntry();

  
  
  
  static bool ExplicitUserRequest(const int load_flags);

  
  base::TimeTicks sliding_window_release_time() const {
    return sliding_window_release_time_;
  }

  
  void set_sliding_window_release_time(const base::TimeTicks& release_time) {
    sliding_window_release_time_ = release_time;
  }

  
  BackoffEntry::Policy backoff_policy_;

 private:
  
  
  
  base::TimeTicks sliding_window_release_time_;

  
  
  std::queue<base::TimeTicks> send_log_;

  const base::TimeDelta sliding_window_period_;
  const int max_send_threshold_;

  
  bool is_backoff_disabled_;

  
  BackoffEntry backoff_entry_;

  
  URLRequestThrottlerManager* manager_;

  
  std::string url_id_;

  BoundNetLog net_log_;

  DISALLOW_COPY_AND_ASSIGN(URLRequestThrottlerEntry);
};

}  

#endif  
