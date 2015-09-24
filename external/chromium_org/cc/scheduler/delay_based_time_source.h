// Copyright 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CC_SCHEDULER_DELAY_BASED_TIME_SOURCE_H_
#define CC_SCHEDULER_DELAY_BASED_TIME_SOURCE_H_

#include "base/memory/weak_ptr.h"
#include "cc/base/cc_export.h"
#include "cc/scheduler/time_source.h"

namespace base { class SingleThreadTaskRunner; }

namespace cc {

class CC_EXPORT DelayBasedTimeSource : public TimeSource {
 public:
  static scoped_refptr<DelayBasedTimeSource> Create(
      base::TimeDelta interval, base::SingleThreadTaskRunner* task_runner);

  virtual void SetClient(TimeSourceClient* client) OVERRIDE;

  
  virtual void SetTimebaseAndInterval(base::TimeTicks timebase,
                                      base::TimeDelta interval) OVERRIDE;

  virtual base::TimeTicks SetActive(bool active) OVERRIDE;
  virtual bool Active() const OVERRIDE;

  
  
  virtual base::TimeTicks LastTickTime() OVERRIDE;
  virtual base::TimeTicks NextTickTime() OVERRIDE;

  
  virtual base::TimeTicks Now() const;

 protected:
  DelayBasedTimeSource(base::TimeDelta interval,
                       base::SingleThreadTaskRunner* task_runner);
  virtual ~DelayBasedTimeSource();

  base::TimeTicks NextTickTarget(base::TimeTicks now);
  void PostNextTickTask(base::TimeTicks now);
  void OnTimerFired();

  struct Parameters {
    Parameters(base::TimeDelta interval, base::TimeTicks tick_target)
        : interval(interval), tick_target(tick_target) {}
    base::TimeDelta interval;
    base::TimeTicks tick_target;
  };

  TimeSourceClient* client_;
  base::TimeTicks last_tick_time_;

  // current_parameters_ should only be written by PostNextTickTask.
  
  
  
  Parameters current_parameters_;
  Parameters next_parameters_;

  bool active_;

  base::SingleThreadTaskRunner* task_runner_;
  base::WeakPtrFactory<DelayBasedTimeSource> weak_factory_;

 private:
  DISALLOW_COPY_AND_ASSIGN(DelayBasedTimeSource);
};

class DelayBasedTimeSourceHighRes : public DelayBasedTimeSource {
 public:
  static scoped_refptr<DelayBasedTimeSourceHighRes> Create(
        base::TimeDelta interval, base::SingleThreadTaskRunner* task_runner);

  virtual base::TimeTicks Now() const OVERRIDE;

 protected:
  DelayBasedTimeSourceHighRes(base::TimeDelta interval,
                              base::SingleThreadTaskRunner* task_runner);
  virtual ~DelayBasedTimeSourceHighRes();

 private:
  DISALLOW_COPY_AND_ASSIGN(DelayBasedTimeSourceHighRes);
};

}  

#endif  
