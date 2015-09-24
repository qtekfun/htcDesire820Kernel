// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_URL_REQUEST_URL_REQUEST_THROTTLER_ENTRY_H_
#define NET_URL_REQUEST_URL_REQUEST_THROTTLER_ENTRY_H_
#pragma once

#include <queue>
#include <string>

#include "base/basictypes.h"
#include "net/base/backoff_entry.h"
#include "net/url_request/url_request_throttler_entry_interface.h"

namespace net {

class URLRequestThrottlerManager;

class URLRequestThrottlerEntry : public URLRequestThrottlerEntryInterface {
 public:
  
  static const int kDefaultSlidingWindowPeriodMs;

  
  static const int kDefaultMaxSendThreshold;

  
  static const int kDefaultNumErrorsToIgnore;

  
  static const int kDefaultInitialBackoffMs;

  
  static const double kDefaultMultiplyFactor;

  
  
  static const double kDefaultJitterFactor;

  
  static const int kDefaultMaximumBackoffMs;

  
  static const int kDefaultEntryLifetimeMs;

  
  
  static const char kRetryHeaderName[];

  
  
  static const char kExponentialThrottlingHeader[];

  
  
  static const char kExponentialThrottlingDisableValue[];

  
  explicit URLRequestThrottlerEntry(URLRequestThrottlerManager* manager);

  
  
  
  URLRequestThrottlerEntry(URLRequestThrottlerManager* manager,
                           int sliding_window_period_ms,
                           int max_send_threshold,
                           int initial_backoff_ms,
                           double multiply_factor,
                           double jitter_factor,
                           int maximum_backoff_ms);

  
  
  bool IsEntryOutdated() const;

  
  void DisableBackoffThrottling();

  
  void DetachManager();

  
  virtual bool IsDuringExponentialBackoff() const;
  virtual int64 ReserveSendingTimeForNextRequest(
      const base::TimeTicks& earliest_time);
  virtual base::TimeTicks GetExponentialBackoffReleaseTime() const;
  virtual void UpdateWithResponse(
      const std::string& host,
      const URLRequestThrottlerHeaderInterface* response);
  virtual void ReceivedContentWasMalformed();

 protected:
  virtual ~URLRequestThrottlerEntry();

  void Initialize();

  
  virtual base::TimeTicks GetTimeNow() const;

  
  void HandleCustomRetryAfter(const std::string& header_value);

  
  void HandleThrottlingHeader(const std::string& header_value,
                              const std::string& host);

  
  
  virtual const BackoffEntry* GetBackoffEntry() const;
  virtual BackoffEntry* GetBackoffEntry();

  
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

  DISALLOW_COPY_AND_ASSIGN(URLRequestThrottlerEntry);
};

}  

#endif  
