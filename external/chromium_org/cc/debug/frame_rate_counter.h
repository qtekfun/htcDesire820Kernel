// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_DEBUG_FRAME_RATE_COUNTER_H_
#define CC_DEBUG_FRAME_RATE_COUNTER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "cc/debug/ring_buffer.h"

namespace cc {

class FrameRateCounter {
 public:
  static scoped_ptr<FrameRateCounter> Create(bool has_impl_thread);

  int current_frame_number() const { return ring_buffer_.CurrentIndex(); }
  int dropped_frame_count() const { return dropped_frame_count_; }
  size_t time_stamp_history_size() const { return ring_buffer_.BufferSize(); }

  void SaveTimeStamp(base::TimeTicks timestamp, bool software);

  
  
  base::TimeDelta RecentFrameInterval(size_t n) const;

  
  
  
  bool IsBadFrameInterval(
      base::TimeDelta interval_between_consecutive_frames) const;

  void GetMinAndMaxFPS(double* min_fps, double* max_fps) const;
  double GetAverageFPS() const;

  typedef RingBuffer<base::TimeTicks, 136> RingBufferType;
  RingBufferType::Iterator begin() const { return ring_buffer_.Begin(); }
  RingBufferType::Iterator end() const { return ring_buffer_.End(); }

 private:
  explicit FrameRateCounter(bool has_impl_thread);

  RingBufferType ring_buffer_;

  bool has_impl_thread_;
  int dropped_frame_count_;

  DISALLOW_COPY_AND_ASSIGN(FrameRateCounter);
};

}  

#endif  
