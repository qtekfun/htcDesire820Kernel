// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_NETWORK_TIME_NOTIFIER_H_
#define NET_BASE_NETWORK_TIME_NOTIFIER_H_

#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"
#include "base/time/tick_clock.h"
#include "base/time/time.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT NetworkTimeNotifier {
 public:
  
  
  
  
  
  
  
  typedef base::Callback<void(const base::Time&,
                              const base::TimeTicks&,
                              const base::TimeDelta&)> ObserverCallback;

  
  explicit NetworkTimeNotifier(scoped_ptr<base::TickClock> tick_clock);
  ~NetworkTimeNotifier();

  
  
  
  
  void UpdateNetworkTime(const base::Time& network_time,
                         const base::TimeDelta& resolution,
                         const base::TimeDelta& latency,
                         const base::TimeTicks& post_time);

  
  
  void AddObserver(const ObserverCallback& observer_callback);

 private:
  base::ThreadChecker thread_checker_;

  
  scoped_ptr<base::TickClock> tick_clock_;

  
  base::Time network_time_;

  
  
  
  
  
  base::TimeTicks network_time_ticks_;

  
  
  base::TimeDelta network_time_uncertainty_;

  
  
  
  std::vector<ObserverCallback> observers_;

  DISALLOW_COPY_AND_ASSIGN(NetworkTimeNotifier);
};

}  

#endif  
