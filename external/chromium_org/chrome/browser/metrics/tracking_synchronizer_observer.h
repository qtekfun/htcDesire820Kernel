// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRICS_TRACKING_SYNCHRONIZER_OBSERVER_H_
#define CHROME_BROWSER_METRICS_TRACKING_SYNCHRONIZER_OBSERVER_H_

namespace tracked_objects {
struct ProcessDataSnapshot;
}

namespace chrome_browser_metrics {

class TrackingSynchronizerObserver {
 public:
  
  
  
  virtual void ReceivedProfilerData(
      const tracked_objects::ProcessDataSnapshot& profiler_data,
      int process_type) = 0;

  
  
  
  virtual void FinishedReceivingProfilerData() {}

 protected:
  TrackingSynchronizerObserver() {}
  virtual ~TrackingSynchronizerObserver() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(TrackingSynchronizerObserver);
};

}  

#endif  
