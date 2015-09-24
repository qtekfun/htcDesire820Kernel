// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_TIME_H_
#define NET_QUIC_QUIC_TIME_H_

#include "base/basictypes.h"
#include "base/time/time.h"
#include "net/base/net_export.h"

namespace net {

static const uint64 kNumMicrosPerSecond = base::Time::kMicrosecondsPerSecond;

class NET_EXPORT_PRIVATE QuicTime {
 public:
  
  
  class NET_EXPORT_PRIVATE Delta {
   public:
    explicit Delta(base::TimeDelta delta);

    
    static Delta Zero();

    
    static Delta Infinite();

    
    static Delta FromSeconds(int64 secs);

    
    static Delta FromMilliseconds(int64 ms);

    
    static Delta FromMicroseconds(int64 us);

    
    int64 ToSeconds() const;

    
    int64 ToMilliseconds() const;

    
    int64 ToMicroseconds() const;

    Delta Add(const Delta& delta) const;

    Delta Subtract(const Delta& delta) const;

    bool IsZero() const;

    bool IsInfinite() const;

   private:
    base::TimeDelta delta_;

    friend class QuicTime;
    friend class QuicClock;
  };

  explicit QuicTime(base::TimeTicks ticks);

  
  
  static QuicTime Zero();

  
  
  
  
  int64 ToDebuggingValue() const;

  bool IsInitialized() const;

  QuicTime Add(const Delta& delta) const;

  QuicTime Subtract(const Delta& delta) const;

  Delta Subtract(const QuicTime& other) const;

 private:
  friend bool operator==(QuicTime lhs, QuicTime rhs);
  friend bool operator<(QuicTime lhs, QuicTime rhs);

  friend class QuicClock;
  friend class QuicClockTest;

  base::TimeTicks ticks_;
};

class NET_EXPORT_PRIVATE QuicWallTime {
 public:
  
  
  static QuicWallTime FromUNIXSeconds(uint64 seconds);

  
  
  static QuicWallTime Zero();

  
  
  uint64 ToUNIXSeconds() const;

  bool IsAfter(QuicWallTime other) const;
  bool IsBefore(QuicWallTime other) const;

  
  bool IsZero() const;

  
  
  QuicTime::Delta AbsoluteDifference(QuicWallTime other) const;

  
  
  QuicWallTime Add(QuicTime::Delta delta) const;

  
  
  QuicWallTime Subtract(QuicTime::Delta delta) const;

 private:
  explicit QuicWallTime(uint64 seconds);

  uint64 seconds_;
};

inline bool operator==(QuicTime::Delta lhs, QuicTime::Delta rhs) {
  return lhs.ToMicroseconds() == rhs.ToMicroseconds();
}
inline bool operator!=(QuicTime::Delta lhs, QuicTime::Delta rhs) {
  return !(lhs == rhs);
}
inline bool operator<(QuicTime::Delta lhs, QuicTime::Delta rhs) {
  return lhs.ToMicroseconds() < rhs.ToMicroseconds();
}
inline bool operator>(QuicTime::Delta lhs, QuicTime::Delta rhs) {
  return rhs < lhs;
}
inline bool operator<=(QuicTime::Delta lhs, QuicTime::Delta rhs) {
  return !(rhs < lhs);
}
inline bool operator>=(QuicTime::Delta lhs, QuicTime::Delta rhs) {
  return !(lhs < rhs);
}
inline bool operator==(QuicTime lhs, QuicTime rhs) {
  return lhs.ticks_ == rhs.ticks_;
}
inline bool operator!=(QuicTime lhs, QuicTime rhs) {
  return !(lhs == rhs);
}
inline bool operator<(QuicTime lhs, QuicTime rhs) {
  return lhs.ticks_ < rhs.ticks_;
}
inline bool operator>(QuicTime lhs, QuicTime rhs) {
  return rhs < lhs;
}
inline bool operator<=(QuicTime lhs, QuicTime rhs) {
  return !(rhs < lhs);
}
inline bool operator>=(QuicTime lhs, QuicTime rhs) {
  return !(lhs < rhs);
}

}  

#endif  
