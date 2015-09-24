// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_FAKE_AUDIO_CONSUMER_H_
#define MEDIA_AUDIO_FAKE_AUDIO_CONSUMER_H_

#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "media/base/media_export.h"

namespace base {
class MessageLoopProxy;
}

namespace media {
class AudioBus;
class AudioParameters;

class MEDIA_EXPORT FakeAudioConsumer {
 public:
  
  
  
  
  FakeAudioConsumer(const scoped_refptr<base::MessageLoopProxy>& worker_loop,
                    const AudioParameters& params);
  ~FakeAudioConsumer();

  
  
  typedef base::Callback<void(AudioBus* audio_bus)> ReadCB;
  void Start(const ReadCB& read_cb);

  
  
  
  void Stop();

 private:
  
  
  
  class Worker;
  const scoped_refptr<Worker> worker_;

  DISALLOW_COPY_AND_ASSIGN(FakeAudioConsumer);
};

}  

#endif  
