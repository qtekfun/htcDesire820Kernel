// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_PULL_FIFO_H_
#define MEDIA_BASE_AUDIO_PULL_FIFO_H_

#include "base/callback.h"
#include "media/base/media_export.h"

namespace media {
class AudioBus;

class MEDIA_EXPORT AudioPullFifo {
 public:
  
  
  
  
  typedef base::Callback<void(int frame_delay, AudioBus* audio_bus)> ReadCB;

  
  
  
  
  AudioPullFifo(int channels, int frames, const ReadCB& read_cb);
  virtual ~AudioPullFifo();

  
  
  
  
  void Consume(AudioBus* destination, int frames_to_consume);

  
  void Clear();

 private:
  
  
  int ReadFromFifo(AudioBus* destination, int frames_to_provide, int write_pos);

  
  const ReadCB read_cb_;

  
  scoped_ptr<AudioBus> fifo_;
  int fifo_index_;

  DISALLOW_COPY_AND_ASSIGN(AudioPullFifo);
};

}  

#endif  
