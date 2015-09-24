// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_CLOCK_H_
#define MEDIA_BASE_CLOCK_H_

#include "base/basictypes.h"
#include "base/time/time.h"
#include "media/base/media_export.h"

namespace base {
class TickClock;
}  

namespace media {

class MEDIA_EXPORT Clock {
 public:
  explicit Clock(base::TickClock* clock);
  ~Clock();

  
  bool IsPlaying() const;

  
  
  base::TimeDelta Play();

  
  
  base::TimeDelta Pause();

  
  
  void SetPlaybackRate(float playback_rate);

  
  
  
  
  
  
  void SetTime(base::TimeDelta current_time, base::TimeDelta max_time);

  
  void SetMaxTime(base::TimeDelta max_time);

  
  
  base::TimeDelta Elapsed();

  
  
  void SetDuration(base::TimeDelta duration);

  
  void Reset();

  
  
  void EndOfStream();

  
  base::TimeDelta Duration() const;

 private:
  
  void UpdateReferencePoints();

  
  void UpdateReferencePoints(base::TimeDelta current_time);

  
  
  base::TimeDelta EstimatedElapsedTime();

  
  
  base::TimeDelta ElapsedViaProvidedTime(const base::TimeTicks& time) const;

  base::TimeDelta ClampToValidTimeRange(base::TimeDelta time) const;

  base::TickClock* const clock_;

  
  bool playing_;

  
  
  bool underflow_;

  
  
  base::TimeTicks reference_;

  
  
  base::TimeDelta media_time_;

  
  float playback_rate_;

  
  base::TimeDelta max_time_;

  
  base::TimeDelta duration_;

  DISALLOW_COPY_AND_ASSIGN(Clock);
};

}  

#endif  
