// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_SOUNDS_AUDIO_STREAM_HANDLER_H_
#define MEDIA_AUDIO_SOUNDS_AUDIO_STREAM_HANDLER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string_piece.h"
#include "base/threading/non_thread_safe.h"
#include "media/audio/audio_io.h"
#include "media/audio/audio_parameters.h"
#include "media/audio/sounds/wav_audio_handler.h"
#include "media/base/media_export.h"

namespace media {

class AudioManager;

class MEDIA_EXPORT AudioStreamHandler : public base::NonThreadSafe {
 public:
  class TestObserver {
   public:
    virtual ~TestObserver() {}

    

    
    virtual void OnPlay() = 0;

    
    virtual void OnStop(size_t cursor) = 0;
  };

  
  
  explicit AudioStreamHandler(const base::StringPiece& wav_data);
  virtual ~AudioStreamHandler();

  
  bool IsInitialized() const;

  
  
  
  
  bool Play();

  
  void Stop();

  const WavAudioHandler& wav_audio_handler() const { return wav_audio_; }

 private:
  friend class AudioStreamHandlerTest;
  friend class SoundsManagerTest;

  class AudioStreamContainer;

  static void SetObserverForTesting(TestObserver* observer);
  static void SetAudioSourceForTesting(
      AudioOutputStream::AudioSourceCallback* source);

  WavAudioHandler wav_audio_;
  scoped_ptr<AudioStreamContainer> stream_;

  bool initialized_;

  DISALLOW_COPY_AND_ASSIGN(AudioStreamHandler);
};

}  

#endif  
