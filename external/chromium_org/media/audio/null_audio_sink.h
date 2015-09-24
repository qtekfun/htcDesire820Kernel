// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_NULL_AUDIO_SINK_H_
#define MEDIA_AUDIO_NULL_AUDIO_SINK_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "media/base/audio_renderer_sink.h"

namespace base {
class MessageLoopProxy;
}

namespace media {
class AudioBus;
class AudioHash;
class FakeAudioConsumer;

class MEDIA_EXPORT NullAudioSink
    : NON_EXPORTED_BASE(public AudioRendererSink) {
 public:
  NullAudioSink(const scoped_refptr<base::MessageLoopProxy>& message_loop);

  
  virtual void Initialize(const AudioParameters& params,
                          RenderCallback* callback) OVERRIDE;
  virtual void Start() OVERRIDE;
  virtual void Stop() OVERRIDE;
  virtual void Pause() OVERRIDE;
  virtual void Play() OVERRIDE;
  virtual bool SetVolume(double volume) OVERRIDE;

  
  void StartAudioHashForTesting();

  
  std::string GetAudioHashForTesting();

 protected:
  virtual ~NullAudioSink();

 private:
  
  void CallRender(AudioBus* audio_bus);

  bool initialized_;
  bool playing_;
  RenderCallback* callback_;

  
  scoped_ptr<AudioHash> audio_hash_;

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  scoped_ptr<FakeAudioConsumer> fake_consumer_;

  DISALLOW_COPY_AND_ASSIGN(NullAudioSink);
};

}  

#endif  
