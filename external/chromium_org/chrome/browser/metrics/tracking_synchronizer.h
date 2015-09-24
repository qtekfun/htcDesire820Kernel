// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRICS_TRACKING_SYNCHRONIZER_H_
#define CHROME_BROWSER_METRICS_TRACKING_SYNCHRONIZER_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/lazy_instance.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/profiler_subscriber.h"


namespace chrome_browser_metrics {

class TrackingSynchronizerObserver;

class TrackingSynchronizer
    : public content::ProfilerSubscriber,
      public base::RefCountedThreadSafe<TrackingSynchronizer> {
 public:
  
  
  
  
  TrackingSynchronizer();

  
  
  
  
  static void FetchProfilerDataAsynchronously(
      const base::WeakPtr<TrackingSynchronizerObserver>& callback_object);

  
  
  

  
  
  virtual void OnPendingProcesses(int sequence_number,
                                  int pending_processes,
                                  bool end) OVERRIDE;

 private:
  friend class base::RefCountedThreadSafe<TrackingSynchronizer>;

  class RequestContext;

  virtual ~TrackingSynchronizer();

  
  
  
  
  virtual void OnProfilerDataCollected(
      int sequence_number,
      const tracked_objects::ProcessDataSnapshot& profiler_data,
      int process_type) OVERRIDE;

  
  
  
  
  int RegisterAndNotifyAllProcesses(
      const base::WeakPtr<TrackingSynchronizerObserver>& callback_object);

  
  
  
  
  
  
  
  void DecrementPendingProcessesAndSendData(
      int sequence_number,
      const tracked_objects::ProcessDataSnapshot& profiler_data,
      int process_type);

  
  
  int GetNextAvailableSequenceNumber();

  
  
  
  
  
  
  
  int last_used_sequence_number_;

  DISALLOW_COPY_AND_ASSIGN(TrackingSynchronizer);
};

}  

#endif  
