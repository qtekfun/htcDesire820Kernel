// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_RENDERER_MIXER_INPUT_H_
#define MEDIA_BASE_AUDIO_RENDERER_MIXER_INPUT_H_

#include <vector>

#include "base/callback.h"
#include "media/base/audio_converter.h"
#include "media/base/audio_renderer_sink.h"

namespace media {

class AudioRendererMixer;

class MEDIA_EXPORT AudioRendererMixerInput
    : NON_EXPORTED_BASE(public AudioRendererSink),
      public AudioConverter::InputCallback {
 public:
  typedef base::Callback<AudioRendererMixer*(
      const AudioParameters& params)> GetMixerCB;
  typedef base::Callback<void(const AudioParameters& params)> RemoveMixerCB;

  AudioRendererMixerInput(
      const GetMixerCB& get_mixer_cb, const RemoveMixerCB& remove_mixer_cb);

  
  virtual void Start() OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Play() OVERRIDE;
  virtual void Pause() OVERRIDE;
  virtual bool SetVolume(double volume) OVERRIDE;
  virtual void Initialize(const AudioParameters& params,
                          AudioRendererSink::RenderCallback* renderer) OVERRIDE;

  
  void OnRenderError();

 protected:
  virtual ~AudioRendererMixerInput();

 private:
  friend class AudioRendererMixerInputTest;

  bool playing_;
  bool initialized_;
  double volume_;

  
  virtual double ProvideInput(AudioBus* audio_bus,
                              base::TimeDelta buffer_delay) OVERRIDE;

  
  
  GetMixerCB get_mixer_cb_;
  RemoveMixerCB remove_mixer_cb_;

  
  AudioParameters params_;

  
  
  AudioRendererMixer* mixer_;

  
  AudioRendererSink::RenderCallback* callback_;

  
  base::Closure error_cb_;

  DISALLOW_COPY_AND_ASSIGN(AudioRendererMixerInput);
};

}  

#endif  
