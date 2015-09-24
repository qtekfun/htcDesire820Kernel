// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_METRICS_STATS_COUNTERS_H_
#define BASE_METRICS_STATS_COUNTERS_H_

#include <string>

#include "base/base_export.h"
#include "base/compiler_specific.h"
#include "base/metrics/stats_table.h"
#include "base/time/time.h"

namespace base {


#define STATS_COUNTER(name, delta) do { \
  base::StatsCounter counter(name); \
  counter.Add(delta); \
} while (0)

#define SIMPLE_STATS_COUNTER(name) STATS_COUNTER(name, 1)

#define RATE_COUNTER(name, duration) do { \
  base::StatsRate hit_count(name); \
  hit_count.AddTime(duration); \
} while (0)

#ifndef NDEBUG

#define DSTATS_COUNTER(name, delta) STATS_COUNTER(name, delta)
#define DSIMPLE_STATS_COUNTER(name) SIMPLE_STATS_COUNTER(name)
#define DRATE_COUNTER(name, duration) RATE_COUNTER(name, duration)

#else  

#define DSTATS_COUNTER(name, delta) do {} while (0)
#define DSIMPLE_STATS_COUNTER(name) do {} while (0)
#define DRATE_COUNTER(name, duration) do {} while (0)

#endif  

class BASE_EXPORT StatsCounter {
 public:
  
  explicit StatsCounter(const std::string& name);
  virtual ~StatsCounter();

  
  void Set(int value);

  
  void Increment() {
    Add(1);
  }

  virtual void Add(int value);

  
  void Decrement() {
    Add(-1);
  }

  void Subtract(int value) {
    Add(-value);
  }

  
  
  bool Enabled() {
    return GetPtr() != NULL;
  }

  int value() {
    int* loc = GetPtr();
    if (loc) return *loc;
    return 0;
  }

 protected:
  StatsCounter();

  
  int* GetPtr();

  std::string name_;
  
  
  
  int32 counter_id_;
};


class BASE_EXPORT StatsCounterTimer : protected StatsCounter {
 public:
  
  explicit StatsCounterTimer(const std::string& name);
  virtual ~StatsCounterTimer();

  
  void Start();

  
  void Stop();

  
  bool Running();

  
  virtual void AddTime(TimeDelta time);

 protected:
  
  void Record();

  TimeTicks start_time_;
  TimeTicks stop_time_;
};

class BASE_EXPORT StatsRate : public StatsCounterTimer {
 public:
  
  explicit StatsRate(const std::string& name);
  virtual ~StatsRate();

  virtual void Add(int value) OVERRIDE;

 private:
  StatsCounter counter_;
  StatsCounter largest_add_;
};


template<class T> class StatsScope {
 public:
  explicit StatsScope<T>(T& timer)
      : timer_(timer) {
    timer_.Start();
  }

  ~StatsScope() {
    timer_.Stop();
  }

  void Stop() {
    timer_.Stop();
  }

 private:
  T& timer_;
};

}  

#endif  
