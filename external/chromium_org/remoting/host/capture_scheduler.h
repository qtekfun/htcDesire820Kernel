// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef REMOTING_HOST_CAPTURE_SCHEDULER_H_
#define REMOTING_HOST_CAPTURE_SCHEDULER_H_

#include "base/time/time.h"
#include "remoting/base/running_average.h"

namespace remoting {

class CaptureScheduler {
 public:
  CaptureScheduler();
  ~CaptureScheduler();

  
  
  base::TimeDelta NextCaptureDelay();

  
  void RecordCaptureTime(base::TimeDelta capture_time);
  void RecordEncodeTime(base::TimeDelta encode_time);

  
  void SetNumOfProcessorsForTest(int num_of_processors);

 private:
  int num_of_processors_;
  RunningAverage capture_time_;
  RunningAverage encode_time_;

  DISALLOW_COPY_AND_ASSIGN(CaptureScheduler);
};

}  

#endif  
