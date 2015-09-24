// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_VIDEO_CAPTURE_ORACLE_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_VIDEO_CAPTURE_ORACLE_H_

#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT SmoothEventSampler {
 public:
  explicit SmoothEventSampler(base::TimeDelta capture_period,
                              bool events_are_reliable,
                              int redundant_capture_goal);

  
  
  
  bool AddEventAndConsiderSampling(base::Time event_time);

  
  
  
  void RecordSample();

  
  
  bool IsOverdueForSamplingAt(base::Time event_time) const;

  
  
  bool HasUnrecordedEvent() const;

 private:
  const bool events_are_reliable_;
  const base::TimeDelta capture_period_;
  const int redundant_capture_goal_;
  const base::TimeDelta token_bucket_capacity_;

  base::Time current_event_;
  base::Time last_sample_;
  int overdue_sample_count_;
  base::TimeDelta token_bucket_;

  DISALLOW_COPY_AND_ASSIGN(SmoothEventSampler);
};

class CONTENT_EXPORT VideoCaptureOracle {
 public:
  enum Event {
    kTimerPoll,
    kCompositorUpdate,
    kSoftwarePaint,
  };

  VideoCaptureOracle(base::TimeDelta capture_period,
                     bool events_are_reliable);
  virtual ~VideoCaptureOracle() {}

  
  
  
  bool ObserveEventAndDecideCapture(
      Event event,
      base::Time event_time);

  
  
  int RecordCapture();

  
  
  bool CompleteCapture(int frame_number, base::Time timestamp);

  base::TimeDelta capture_period() const { return capture_period_; }

 private:

  
  const base::TimeDelta capture_period_;

  
  int frame_number_;

  
  int last_delivered_frame_number_;

  
  base::Time last_delivered_frame_timestamp_;

  
  SmoothEventSampler sampler_;
};

}  

#endif  
