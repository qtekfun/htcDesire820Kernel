// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_NETWORK_TIME_NETWORK_TIME_TRACKER_H_
#define CHROME_BROWSER_NETWORK_TIME_NETWORK_TIME_TRACKER_H_

#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"
#include "base/time/time.h"
#include "net/base/network_time_notifier.h"

class IOThread;
class NetworkTimeTrackerTest;

class NetworkTimeTracker {
 public:
  
  
  
  
  
  
  typedef base::Callback<void(const base::Time&,
                              const base::TimeDelta&,
                              const base::TimeDelta&)> UpdateCallback;

  NetworkTimeTracker();
  ~NetworkTimeTracker();

  
  void Start();

  struct TimeMapping {
    TimeMapping(base::Time local_time, base::Time network_time);
    base::Time local_time;
    base::Time network_time;
  };

  
  
  void InitFromSavedTime(const TimeMapping& saved);

  
  
  
  bool GetNetworkTime(const base::TimeTicks& time_ticks,
                      base::Time* network_time,
                      base::TimeDelta* uncertainty) const;

  
  
  
  static UpdateCallback BuildNotifierUpdateCallback();

  bool received_network_time() const {
    return received_network_time_;
  }

 private:
  friend class NetworkTimeTrackerTest;

  
  
  net::NetworkTimeNotifier::ObserverCallback BuildObserverCallback();

  
  void OnNetworkTimeUpdate(
      const base::Time& network_time,
      const base::TimeTicks& network_time_ticks,
      const base::TimeDelta& network_time_uncertainty);

  
  base::Time network_time_;

  
  
  base::TimeTicks network_time_ticks_;

  
  base::TimeDelta network_time_uncertainty_;

  base::WeakPtrFactory<NetworkTimeTracker> weak_ptr_factory_;

  base::ThreadChecker thread_checker_;

  bool received_network_time_;

  DISALLOW_COPY_AND_ASSIGN(NetworkTimeTracker);
};

#endif  
