// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_RENDERER_SINK_H_
#define MEDIA_BASE_AUDIO_RENDERER_SINK_H_

#include <vector>
#include "base/basictypes.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "media/audio/audio_parameters.h"
#include "media/base/audio_bus.h"
#include "media/base/media_export.h"

namespace media {


class AudioRendererSink
    : public base::RefCountedThreadSafe<media::AudioRendererSink> {
 public:
  class RenderCallback {
   public:
    
    
    virtual int Render(AudioBus* dest, int audio_delay_milliseconds) = 0;

    
    virtual void RenderIO(AudioBus* source,
                          AudioBus* dest,
                          int audio_delay_milliseconds) {}

    
    virtual void OnRenderError() = 0;

   protected:
    virtual ~RenderCallback() {}
  };

  
  
  virtual void Initialize(const AudioParameters& params,
                          RenderCallback* callback) = 0;

  
  virtual void Start() = 0;

  
  virtual void Stop() = 0;

  
  virtual void Pause() = 0;

  
  virtual void Play() = 0;

  
  
  virtual bool SetVolume(double volume) = 0;

 protected:
  friend class base::RefCountedThreadSafe<AudioRendererSink>;
  virtual ~AudioRendererSink() {}
};

}  

#endif  
