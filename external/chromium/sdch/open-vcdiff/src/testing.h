// Copyright 2008 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_TESTING_H_
#define OPEN_VCDIFF_TESTING_H_

#include <config.h>
#include <assert.h>
#include <stdint.h>  
#include <stdlib.h>  
#include <time.h>  
#include "gtest/gtest.h"

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>  
#endif  

#ifdef HAVE_WINDOWS_H
#include <windows.h>  
#endif  


#define CHECK(CONDITION) assert(CONDITION)
#define CHECK_EQ(X, Y) assert(X == Y)
#define CHECK_NE(X, Y) assert(X != Y)
#define CHECK_GE(X, Y) assert(X >= Y)
#define CHECK_GT(X, Y) assert(X > Y)
#define CHECK_LE(X, Y) assert(X <= Y)
#define CHECK_LT(X, Y) assert(X < Y)

namespace open_vcdiff {

#if defined(HAVE_GETTIMEOFDAY)
class CycleTimer {
 public:
  inline CycleTimer() {
    Reset();
  }

  inline void Reset() {
    start_time_.tv_sec = 0;
    start_time_.tv_usec = 0;
    cumulative_time_in_usec_ = 0;
  }

  inline void Start() {
    CHECK(!IsStarted());
    gettimeofday(&start_time_, NULL);
  }

  inline void Restart() {
    Reset();
    Start();
  }

  inline void Stop() {
    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    CHECK(IsStarted());
    cumulative_time_in_usec_ +=
        (1000000 * (end_time.tv_sec - start_time_.tv_sec))
        + end_time.tv_usec - start_time_.tv_usec;
    start_time_.tv_sec = 0;
    start_time_.tv_usec = 0;
  }

  inline int64_t GetInUsec() {
    return cumulative_time_in_usec_;
  }

 private:
  inline bool IsStarted() {
    return (start_time_.tv_usec > 0) || (start_time_.tv_sec > 0);
  }

  struct timeval start_time_;
  int64_t cumulative_time_in_usec_;
};
#elif defined(HAVE_QUERYPERFORMANCECOUNTER)
class CycleTimer {
 public:
  inline CycleTimer() {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);  
    usecs_per_count_ = 1000000.0 / static_cast<double>(frequency.QuadPart);
    Reset();
  }

  inline void Reset() {
    start_time_.QuadPart = 0;
    cumulative_time_in_usec_ = 0;
  }

  inline void Start() {
    CHECK(!IsStarted());
    QueryPerformanceCounter(&start_time_);
  }

  inline void Restart() {
    Reset();
    Start();
  }

  inline void Stop() {
    LARGE_INTEGER end_time;
    QueryPerformanceCounter(&end_time);
    CHECK(IsStarted());
    double count_diff = static_cast<double>(
        end_time.QuadPart - start_time_.QuadPart);
    cumulative_time_in_usec_ +=
        static_cast<int64_t>(count_diff * usecs_per_count_);
    start_time_.QuadPart = 0;
  }

  inline int64_t GetInUsec() {
    return cumulative_time_in_usec_;
  }

 private:
  inline bool IsStarted() {
    return start_time_.QuadPart > 0;
  }

  LARGE_INTEGER start_time_;
  int64_t cumulative_time_in_usec_;
  double usecs_per_count_;
};
#else
#error CycleTimer needs an implementation that does not use gettimeofday or QueryPerformanceCounter
#endif  

template<typename IntType>
inline IntType PortableRandomInRange(IntType limit) {
  uint64_t value = rand();
  double rand_limit = RAND_MAX;  
  while (rand_limit < limit) {
    
    
    value = (value * (static_cast<uint64_t>(RAND_MAX) + 1)) + rand();
    rand_limit = (rand_limit * (RAND_MAX + 1.0)) + RAND_MAX;
  }
  
  
  const double scaled_value = value / rand_limit;
  return static_cast<IntType>(limit * scaled_value);
}

}  

#endif  
