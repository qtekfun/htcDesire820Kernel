// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_BACKOFF_ENTRY_H_
#define NET_BASE_BACKOFF_ENTRY_H_

#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT BackoffEntry : NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  
  struct Policy {
    
    
    int num_errors_to_ignore;

    
    
    
    
    int initial_delay_ms;

    
    double multiply_factor;

    
    
    double jitter_factor;

    
    
    int64 maximum_backoff_ms;

    
    
    int64 entry_lifetime_ms;

    
    
    
    
    
    
    
    bool always_use_initial_delay;
  };

  
  
  explicit BackoffEntry(const Policy* const policy);
  virtual ~BackoffEntry();

  
  
  void InformOfRequest(bool succeeded);

  
  
  bool ShouldRejectRequest() const;

  
  
  base::TimeTicks GetReleaseTime() const;

  
  base::TimeDelta GetTimeUntilRelease() const;

  
  
  void SetCustomReleaseTime(const base::TimeTicks& release_time);

  
  
  
  bool CanDiscard() const;

  
  void Reset();

  
  int failure_count() const { return failure_count_; }

 protected:
  
  virtual base::TimeTicks ImplGetTimeNow() const;

 private:
  
  base::TimeTicks CalculateReleaseTime() const;

  
  
  base::TimeTicks exponential_backoff_release_time_;

  
  int failure_count_;

  const Policy* const policy_;

  DISALLOW_COPY_AND_ASSIGN(BackoffEntry);
};

}  

#endif  
