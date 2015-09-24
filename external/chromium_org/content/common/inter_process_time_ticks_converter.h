// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_INTER_PROCESS_TIME_TICKS_CONVERTER_H_
#define CONTENT_COMMON_INTER_PROCESS_TIME_TICKS_CONVERTER_H_

#include "base/time/time.h"
#include "content/common/content_export.h"

namespace content {

class LocalTimeDelta;
class LocalTimeTicks;
class RemoteTimeDelta;
class RemoteTimeTicks;

class CONTENT_EXPORT InterProcessTimeTicksConverter {
 public:
  InterProcessTimeTicksConverter(const LocalTimeTicks& local_lower_bound,
                                 const LocalTimeTicks& local_upper_bound,
                                 const RemoteTimeTicks& remote_lower_bound,
                                 const RemoteTimeTicks& remote_upper_bound);

  
  
  LocalTimeTicks ToLocalTimeTicks(const RemoteTimeTicks& remote_ms) const;

  
  
  LocalTimeDelta ToLocalTimeDelta(const RemoteTimeDelta& remote_delta) const;

 private:
  int64 Convert(int64 value) const;

  
  int64 local_base_time_;

  int64 numerator_;
  int64 denominator_;

  int64 remote_lower_bound_;
  int64 remote_upper_bound_;
};

class CONTENT_EXPORT LocalTimeDelta {
 public:
  int ToInt32() const { return value_; }

 private:
  friend class InterProcessTimeTicksConverter;
  friend class LocalTimeTicks;

  LocalTimeDelta(int value) : value_(value) {}

  int value_;
};

class CONTENT_EXPORT LocalTimeTicks {
 public:
  static LocalTimeTicks FromTimeTicks(const base::TimeTicks& value) {
    return LocalTimeTicks(value.ToInternalValue());
  }

  base::TimeTicks ToTimeTicks() {
    return base::TimeTicks::FromInternalValue(value_);
  }

  LocalTimeTicks operator+(const LocalTimeDelta& delta) {
    return LocalTimeTicks(value_ + delta.value_);
  }

 private:
  friend class InterProcessTimeTicksConverter;

  LocalTimeTicks(int64 value) : value_(value) {}

  int64 value_;
};

class CONTENT_EXPORT RemoteTimeDelta {
 public:
  static RemoteTimeDelta FromRawDelta(int delta) {
    return RemoteTimeDelta(delta);
  }

 private:
  friend class InterProcessTimeTicksConverter;
  friend class RemoteTimeTicks;

  RemoteTimeDelta(int value) : value_(value) {}

  int value_;
};

class CONTENT_EXPORT RemoteTimeTicks {
 public:
  static RemoteTimeTicks FromTimeTicks(const base::TimeTicks& ticks) {
    return RemoteTimeTicks(ticks.ToInternalValue());
  }

  RemoteTimeDelta operator-(const RemoteTimeTicks& rhs) const {
    return RemoteTimeDelta(value_ - rhs.value_);
  }

 private:
  friend class InterProcessTimeTicksConverter;

  RemoteTimeTicks(int64 value) : value_(value) {}

  int64 value_;
};

}  

#endif  
