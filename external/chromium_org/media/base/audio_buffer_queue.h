// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_BUFFER_QUEUE_H_
#define MEDIA_BASE_AUDIO_BUFFER_QUEUE_H_

#include <deque>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "media/base/audio_buffer.h"
#include "media/base/media_export.h"

namespace media {

class AudioBus;

class MEDIA_EXPORT AudioBufferQueue {
 public:
  AudioBufferQueue();
  ~AudioBufferQueue();

  
  void Clear();

  
  void Append(const scoped_refptr<AudioBuffer>& buffer_in);

  
  
  
  
  
  int ReadFrames(int frames, int dest_frame_offset, AudioBus* dest);

  
  
  
  
  
  int PeekFrames(int frames,
                 int source_frame_offset,
                 int dest_frame_offset,
                 AudioBus* dest);

  
  
  void SeekFrames(int frames);

  
  int frames() const { return frames_; }

  
  
  
  
  
  base::TimeDelta current_time() const { return current_time_; }

 private:
  
  typedef std::deque<scoped_refptr<AudioBuffer> > BufferQueue;

  
  
  
  
  
  
  
  int InternalRead(int frames,
                   bool advance_position,
                   int source_frame_offset,
                   int dest_frame_offset,
                   AudioBus* dest);

  
  
  void UpdateCurrentTime(BufferQueue::iterator buffer, int offset);

  BufferQueue::iterator current_buffer_;
  BufferQueue buffers_;
  int current_buffer_offset_;

  
  int frames_;

  
  
  base::TimeDelta current_time_;

  DISALLOW_COPY_AND_ASSIGN(AudioBufferQueue);
};

}  

#endif  
