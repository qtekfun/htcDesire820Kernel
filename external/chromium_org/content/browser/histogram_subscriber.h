// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_HISTOGRAM_SUBSCRIBER_H_
#define CONTENT_BROWSER_HISTOGRAM_SUBSCRIBER_H_

#include <string>
#include <vector>

namespace content {

class HistogramSubscriber {
 public:
  virtual ~HistogramSubscriber() {}

  
  
  virtual void OnPendingProcesses(int sequence_number,
                                  int pending_processes,
                                  bool end) = 0;

  
  
  virtual void OnHistogramDataCollected(
      int sequence_number,
      const std::vector<std::string>& pickled_histograms) = 0;
};

}  

#endif  
