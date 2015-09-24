// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SPEECH_AUDIO_BUFFER_H_
#define CONTENT_BROWSER_SPEECH_AUDIO_BUFFER_H_

#include <deque>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"

namespace content {

class CONTENT_EXPORT AudioChunk :
    public base::RefCountedThreadSafe<AudioChunk> {
 public:
  explicit AudioChunk(int bytes_per_sample);
  AudioChunk(const uint8* data, size_t length, int bytes_per_sample);

  bool IsEmpty() const;
  int bytes_per_sample() const { return bytes_per_sample_; }
  size_t NumSamples() const;
  const std::string& AsString() const;
  int16 GetSample16(size_t index) const;
  const int16* SamplesData16() const;
  friend class AudioBuffer;

 private:
  ~AudioChunk() {}
  friend class base::RefCountedThreadSafe<AudioChunk>;

  std::string data_string_;
  int bytes_per_sample_;

  DISALLOW_COPY_AND_ASSIGN(AudioChunk);
};

class AudioBuffer {
 public:
  explicit AudioBuffer(int bytes_per_sample);
  ~AudioBuffer();

  
  void Enqueue(const uint8* data, size_t length);

  
  
  
  scoped_refptr<AudioChunk> DequeueSingleChunk();

  
  scoped_refptr<AudioChunk> DequeueAll();

  
  void Clear();

  
  bool IsEmpty() const;

 private:
  typedef std::deque<scoped_refptr<AudioChunk> > ChunksContainer;
  ChunksContainer chunks_;
  int bytes_per_sample_;

  DISALLOW_COPY_AND_ASSIGN(AudioBuffer);
};

}  

#endif  
