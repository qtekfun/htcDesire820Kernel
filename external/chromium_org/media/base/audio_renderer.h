// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_RENDERER_H_
#define MEDIA_BASE_AUDIO_RENDERER_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/time/time.h"
#include "media/base/media_export.h"
#include "media/base/pipeline_status.h"

namespace media {

class DemuxerStream;

class MEDIA_EXPORT AudioRenderer {
 public:
  
  
  typedef base::Callback<void(base::TimeDelta, base::TimeDelta)> TimeCB;

  AudioRenderer();
  virtual ~AudioRenderer();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void Initialize(DemuxerStream* stream,
                          const PipelineStatusCB& init_cb,
                          const StatisticsCB& statistics_cb,
                          const base::Closure& underflow_cb,
                          const TimeCB& time_cb,
                          const base::Closure& ended_cb,
                          const base::Closure& disabled_cb,
                          const PipelineStatusCB& error_cb) = 0;

  
  
  virtual void Play(const base::Closure& callback) = 0;

  
  
  virtual void Pause(const base::Closure& callback) = 0;

  
  virtual void Flush(const base::Closure& callback) = 0;

  
  
  
  
  virtual void Preroll(base::TimeDelta time,
                       const PipelineStatusCB& callback) = 0;

  
  
  virtual void Stop(const base::Closure& callback) = 0;

  
  virtual void SetPlaybackRate(float playback_rate) = 0;

  
  virtual void SetVolume(float volume) = 0;

  
  virtual void ResumeAfterUnderflow() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(AudioRenderer);
};

}  

#endif  
