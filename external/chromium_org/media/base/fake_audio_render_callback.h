// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_FAKE_AUDIO_RENDER_CALLBACK_H_
#define MEDIA_BASE_FAKE_AUDIO_RENDER_CALLBACK_H_

#include "media/base/audio_converter.h"
#include "media/base/audio_renderer_sink.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace media {

class FakeAudioRenderCallback
    : public AudioRendererSink::RenderCallback,
      public AudioConverter::InputCallback {
 public:
  
  
  
  explicit FakeAudioRenderCallback(double step);
  virtual ~FakeAudioRenderCallback();

  
  
  virtual int Render(AudioBus* audio_bus,
                     int audio_delay_milliseconds) OVERRIDE;
  MOCK_METHOD0(OnRenderError, void());

  
  virtual double ProvideInput(AudioBus* audio_bus,
                              base::TimeDelta buffer_delay) OVERRIDE;

  
  void set_half_fill(bool half_fill) { half_fill_ = half_fill; }

  
  void reset() { x_ = 0; }

  
  
  int last_audio_delay_milliseconds() { return last_audio_delay_milliseconds_; }

  
  void set_volume(double volume) { volume_ = volume; }

 private:
  bool half_fill_;
  double x_;
  double step_;
  int last_audio_delay_milliseconds_;
  double volume_;

  DISALLOW_COPY_AND_ASSIGN(FakeAudioRenderCallback);
};

}  

#endif  
