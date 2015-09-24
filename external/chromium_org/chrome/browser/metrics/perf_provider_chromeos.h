// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRICS_PERF_PROVIDER_CHROMEOS_H_
#define CHROME_BROWSER_METRICS_PERF_PROVIDER_CHROMEOS_H_

#include <string>

#include "base/basictypes.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/common/metrics/proto/perf_data.pb.h"

namespace metrics {

class WindowedIncognitoObserver;

class PerfProvider : public base::NonThreadSafe {
 public:
  PerfProvider();
  ~PerfProvider();

  
  
  bool GetPerfData(PerfDataProto* perf_data_proto);

 private:
  enum PerfDataState {
    
    READY_TO_COLLECT,

    
    READY_TO_UPLOAD,
  };

  
  
  void ScheduleCollection(const base::TimeDelta& interval);

  
  
  void CollectIfNecessary();

  
  
  void CollectIfNecessaryAndReschedule();

  
  
  
  void ParseProtoIfValid(
      scoped_ptr<WindowedIncognitoObserver> incognito_observer,
      const std::vector<uint8>& data);

  
  PerfDataState state_;

  
  PerfDataProto perf_data_proto_;

  
  base::OneShotTimer<PerfProvider> timer_;

  
  base::WeakPtrFactory<PerfProvider> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PerfProvider);
};

}  

#endif  
