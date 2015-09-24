// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_HISTOGRAM_SYNCHRONIZER_H_
#define CONTENT_BROWSER_HISTOGRAM_SYNCHRONIZER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/singleton.h"
#include "base/synchronization/lock.h"
#include "base/time/time.h"
#include "content/browser/histogram_subscriber.h"

namespace base {
class MessageLoop;
}

namespace content {


class HistogramSynchronizer : public HistogramSubscriber {
 public:
  enum ProcessHistogramRequester {
    UNKNOWN,
    ASYNC_HISTOGRAMS,
  };

  
  
  static HistogramSynchronizer* GetInstance();

  
  
  static void FetchHistograms();

  
  
  
  
  static void FetchHistogramsAsynchronously(base::MessageLoop* callback_thread,
                                            const base::Closure& callback,
                                            base::TimeDelta wait_time);

 private:
  friend struct DefaultSingletonTraits<HistogramSynchronizer>;

  class RequestContext;

  HistogramSynchronizer();
  virtual ~HistogramSynchronizer();

  
  
  
  
  void RegisterAndNotifyAllProcesses(ProcessHistogramRequester requester,
                                     base::TimeDelta wait_time);

  
  
  

  
  
  virtual void OnPendingProcesses(int sequence_number,
                                  int pending_processes,
                                  bool end) OVERRIDE;

  
  
  
  virtual void OnHistogramDataCollected(
      int sequence_number,
      const std::vector<std::string>& pickled_histograms) OVERRIDE;

  
  
  
  
  
  void SetCallbackTaskAndThread(base::MessageLoop* callback_thread,
                                const base::Closure& callback);

  void ForceHistogramSynchronizationDoneCallback(int sequence_number);

  
  void InternalPostTask(base::MessageLoop* thread,
                        const base::Closure& callback);

  
  int GetNextAvailableSequenceNumber(ProcessHistogramRequester requester);

  
  base::Lock lock_;

  
  
  
  base::Closure callback_;
  base::MessageLoop* callback_thread_;

  
  
  
  
  
  
  
  
  int last_used_sequence_number_;

  
  
  int async_sequence_number_;

  DISALLOW_COPY_AND_ASSIGN(HistogramSynchronizer);
};

}  

#endif  
