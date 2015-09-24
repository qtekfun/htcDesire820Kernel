// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_HISTOGRAM_CONTROLLER_H_
#define CONTENT_BROWSER_HISTOGRAM_CONTROLLER_H_

#include <string>
#include <vector>

#include "base/memory/singleton.h"

namespace content {

class HistogramSubscriber;

class HistogramController {
 public:
  
  
  static HistogramController* GetInstance();

  
  
  HistogramController();
  virtual ~HistogramController();

  
  
  
  void Register(HistogramSubscriber* subscriber);

  
  
  
  void Unregister(const HistogramSubscriber* subscriber);

  
  void GetHistogramData(int sequence_number);

  
  
  
  
  
  void OnPendingProcesses(int sequence_number, int pending_processes, bool end);

  
  
  void OnHistogramDataCollected(
      int sequence_number,
      const std::vector<std::string>& pickled_histograms);

 private:
  friend struct DefaultSingletonTraits<HistogramController>;

  
  
  
  void GetHistogramDataFromChildProcesses(int sequence_number);

  HistogramSubscriber* subscriber_;

  DISALLOW_COPY_AND_ASSIGN(HistogramController);
};

}  

#endif  
