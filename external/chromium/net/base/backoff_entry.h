// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_BACKOFF_ITEM_H_
#define NET_BASE_BACKOFF_ITEM_H_
#pragma once

#include "base/threading/non_thread_safe.h"
#include "base/time.h"

namespace net {

class BackoffEntry : public base::NonThreadSafe {
 public:
  
  struct Policy {
    
    
    int num_errors_to_ignore;

    
    int initial_backoff_ms;

    
    double multiply_factor;

    
    
    double jitter_factor;

    
    int maximum_backoff_ms;

    
    
    int entry_lifetime_ms;
  };

  
  
  explicit BackoffEntry(const Policy* const policy);
  virtual ~BackoffEntry();

  
  
  void InformOfRequest(bool succeeded);

  
  
  bool ShouldRejectRequest() const;

  
  
  base::TimeTicks GetReleaseTime() const;

  
  
  void SetCustomReleaseTime(const base::TimeTicks& release_time);

  
  
  
  bool CanDiscard() const;

 protected:
  
  
  virtual base::TimeTicks GetTimeNow() const;

 private:
  
  base::TimeTicks CalculateReleaseTime() const;

  
  
  base::TimeTicks exponential_backoff_release_time_;

  
  int failure_count_;

  const Policy* const policy_;

  DISALLOW_COPY_AND_ASSIGN(BackoffEntry);
};

}  

#endif  
