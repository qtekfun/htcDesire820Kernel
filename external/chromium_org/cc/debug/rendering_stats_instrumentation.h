// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_DEBUG_RENDERING_STATS_INSTRUMENTATION_H_
#define CC_DEBUG_RENDERING_STATS_INSTRUMENTATION_H_

#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "cc/debug/rendering_stats.h"

namespace cc {

class CC_EXPORT RenderingStatsInstrumentation {
 public:
  static scoped_ptr<RenderingStatsInstrumentation> Create();
  virtual ~RenderingStatsInstrumentation();

  
  const MainThreadRenderingStats& main_thread_rendering_stats() {
    return main_stats_;
  }
  
  const ImplThreadRenderingStats& impl_thread_rendering_stats() {
    return impl_stats_;
  }
  
  RenderingStats GetRenderingStats();

  
  
  void AccumulateAndClearMainThreadStats();
  
  
  void AccumulateAndClearImplThreadStats();

  
  
  
  bool record_rendering_stats() const { return record_rendering_stats_; }
  void set_record_rendering_stats(bool record_rendering_stats) {
    if (record_rendering_stats_ != record_rendering_stats)
      record_rendering_stats_ = record_rendering_stats;
  }

  base::TimeTicks StartRecording() const;
  base::TimeDelta EndRecording(base::TimeTicks start_time) const;

  void IncrementFrameCount(int64 count, bool main_thread);
  void AddPaint(base::TimeDelta duration, int64 pixels);
  void AddRecord(base::TimeDelta duration, int64 pixels);
  void AddRaster(base::TimeDelta duration, int64 pixels);
  void AddAnalysis(base::TimeDelta duration, int64 pixels);

 protected:
  RenderingStatsInstrumentation();

 private:
  
  MainThreadRenderingStats main_stats_;
  MainThreadRenderingStats main_stats_accu_;
  ImplThreadRenderingStats impl_stats_;
  ImplThreadRenderingStats impl_stats_accu_;

  bool record_rendering_stats_;

  base::Lock lock_;

  DISALLOW_COPY_AND_ASSIGN(RenderingStatsInstrumentation);
};

}  

#endif  
