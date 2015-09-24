// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_CONVERTER_H_
#define MEDIA_BASE_AUDIO_CONVERTER_H_

#include <list>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "media/audio/audio_parameters.h"
#include "media/base/media_export.h"

namespace media {

class AudioBus;
class AudioPullFifo;
class ChannelMixer;
class MultiChannelResampler;

class MEDIA_EXPORT AudioConverter {
 public:
  
  
  class MEDIA_EXPORT InputCallback {
   public:
    
    
    
    
    
    
    virtual double ProvideInput(AudioBus* audio_bus,
                                base::TimeDelta buffer_delay) = 0;

   protected:
    virtual ~InputCallback() {}
  };

  
  
  
  
  
  
  AudioConverter(const AudioParameters& input_params,
                 const AudioParameters& output_params,
                 bool disable_fifo);
  ~AudioConverter();

  
  
  
  void Convert(AudioBus* dest);
  void ConvertWithDelay(const base::TimeDelta& initial_delay, AudioBus* dest);

  
  
  void AddInput(InputCallback* input);
  void RemoveInput(InputCallback* input);

  
  void Reset();

 private:
  
  
  void ProvideInput(int resampler_frame_delay, AudioBus* audio_bus);

  
  
  void SourceCallback(int fifo_frame_delay, AudioBus* audio_bus);

  
  typedef std::list<InputCallback*> InputCallbackSet;
  InputCallbackSet transform_inputs_;

  
  
  scoped_ptr<AudioPullFifo> audio_fifo_;

  
  scoped_ptr<MultiChannelResampler> resampler_;

  
  
  scoped_ptr<ChannelMixer> channel_mixer_;
  scoped_ptr<AudioBus> unmixed_audio_;

  
  scoped_ptr<AudioBus> mixer_input_audio_bus_;

  
  
  bool downmix_early_;

  
  base::TimeDelta input_frame_duration_;
  base::TimeDelta output_frame_duration_;
  base::TimeDelta initial_delay_;
  int resampler_frame_delay_;

  
  
  
  const int input_channel_count_;

  DISALLOW_COPY_AND_ASSIGN(AudioConverter);
};

}  

#endif  
