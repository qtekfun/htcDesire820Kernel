// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FRAME_COUNTER_H_
#define FRAME_COUNTER_H_

class FrameCounter {
 public:
  FrameCounter()
      : frame_duration_accumulator_(0),
        frame_count_(0),
        frames_per_second_(0) {}
  ~FrameCounter() {}

  
  
  void BeginFrame();

  
  
  
  
  void EndFrame();

  
  void Reset();

  
  
  
  double frames_per_second() const {
    return frames_per_second_;
  }

 private:
  static const double kMicroSecondsPerSecond = 1000000.0;
  static const int32_t kFrameRateRefreshCount = 100;

  double frame_duration_accumulator_;  
  int32_t frame_count_;
  double frame_start_;
  double frames_per_second_;
};

#endif  
