// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_TIMESTAMP_HELPER_H_
#define MEDIA_BASE_AUDIO_TIMESTAMP_HELPER_H_

#include "base/time/time.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT AudioTimestampHelper {
 public:
  AudioTimestampHelper(int samples_per_second);

  
  void SetBaseTimestamp(base::TimeDelta base_timestamp);

  base::TimeDelta base_timestamp() const;

  
  
  
  void AddFrames(int frame_count);

  
  
  base::TimeDelta GetTimestamp() const;

  
  
  
  
  base::TimeDelta GetFrameDuration(int frame_count) const;

  
  
  int64 GetFramesToTarget(base::TimeDelta target) const;

 private:
  base::TimeDelta ComputeTimestamp(int64 frame_count) const;

  double microseconds_per_frame_;

  base::TimeDelta base_timestamp_;

  
  int64 frame_count_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(AudioTimestampHelper);
};

}  

#endif
