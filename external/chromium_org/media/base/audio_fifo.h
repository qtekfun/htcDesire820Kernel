// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_FIFO_H_
#define MEDIA_BASE_AUDIO_FIFO_H_

#include "base/atomicops.h"
#include "media/base/audio_bus.h"
#include "media/base/media_export.h"

namespace media {

class MEDIA_EXPORT AudioFifo {
 public:
  
  AudioFifo(int channels, int frames);
  virtual ~AudioFifo();

  
  
  void Push(const AudioBus* source);

  
  
  
  
  
  void Consume(AudioBus* destination, int start_frame, int frames_to_consume);

  
  void Clear();

  
  int frames() const;

  int max_frames() const { return max_frames_; }

 private:
  
  scoped_ptr<AudioBus> audio_bus_;

  
  
  const int max_frames_;

  
  volatile base::subtle::Atomic32 frames_pushed_;
  volatile base::subtle::Atomic32 frames_consumed_;

  
  int read_pos_;

  
  int write_pos_;

  DISALLOW_COPY_AND_ASSIGN(AudioFifo);
};

}  

#endif  
