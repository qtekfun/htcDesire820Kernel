// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_CAPTURER_SOURCE_H_
#define MEDIA_BASE_AUDIO_CAPTURER_SOURCE_H_

#include <string>
#include <vector>
#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "media/audio/audio_parameters.h"
#include "media/base/audio_bus.h"
#include "media/base/media_export.h"

namespace media {

class AudioCapturerSource
    : public base::RefCountedThreadSafe<media::AudioCapturerSource> {
 public:
  class CaptureCallback {
   public:
    
    virtual void Capture(AudioBus* audio_source,
                         int audio_delay_milliseconds,
                         double volume,
                         bool key_pressed) = 0;

    
    virtual void OnCaptureError() = 0;

   protected:
    virtual ~CaptureCallback() {}
  };

  
  
  
  
  
  
  virtual void Initialize(const AudioParameters& params,
                          CaptureCallback* callback,
                          int session_id) = 0;

  
  virtual void Start() = 0;

  
  
  virtual void Stop() = 0;

  
  virtual void SetVolume(double volume) = 0;

  
  virtual void SetAutomaticGainControl(bool enable) = 0;

 protected:
  friend class base::RefCountedThreadSafe<AudioCapturerSource>;
  virtual ~AudioCapturerSource() {}
};

}  

#endif  
