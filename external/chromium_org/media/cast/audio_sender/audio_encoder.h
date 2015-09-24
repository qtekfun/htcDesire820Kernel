// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_AUDIO_SENDER_AUDIO_ENCODER_H_
#define MEDIA_CAST_AUDIO_SENDER_AUDIO_ENCODER_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/thread_checker.h"
#include "media/cast/cast_config.h"
#include "media/cast/cast_environment.h"

namespace base {
class TimeTicks;
}

namespace media {
class AudioBus;
}

namespace media {
namespace cast {

class AudioEncoder : public base::RefCountedThreadSafe<AudioEncoder> {
 public:
  typedef base::Callback<void(scoped_ptr<EncodedAudioFrame>,
                              const base::TimeTicks&)> FrameEncodedCallback;

  AudioEncoder(const scoped_refptr<CastEnvironment>& cast_environment,
               const AudioSenderConfig& audio_config,
               const FrameEncodedCallback& frame_encoded_callback);

  
  
  
  
  void InsertAudio(const AudioBus* audio_bus,
                   const base::TimeTicks& recorded_time,
                   const base::Closure& done_callback);

 protected:
  virtual ~AudioEncoder();

 private:
  friend class base::RefCountedThreadSafe<AudioEncoder>;

  class ImplBase;
  class OpusImpl;
  class Pcm16Impl;

  
  
  void EncodeAudio(const AudioBus* audio_bus,
                   const base::TimeTicks& recorded_time,
                   const base::Closure& done_callback);

  const scoped_refptr<CastEnvironment> cast_environment_;
  scoped_ptr<ImplBase> impl_;

  
  base::ThreadChecker insert_thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(AudioEncoder);
};

}  
}  

#endif  
