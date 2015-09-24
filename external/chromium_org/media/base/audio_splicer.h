// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_SPLICER_H_
#define MEDIA_BASE_AUDIO_SPLICER_H_

#include <deque>

#include "base/memory/ref_counted.h"
#include "media/base/audio_timestamp_helper.h"
#include "media/base/media_export.h"

namespace media {

class AudioBuffer;
class AudioDecoderConfig;

class MEDIA_EXPORT AudioSplicer {
 public:
  AudioSplicer(int samples_per_second);
  ~AudioSplicer();

  
  
  void Reset();

  
  
  
  bool AddInput(const scoped_refptr<AudioBuffer>& input);

  
  bool HasNextBuffer() const;

  
  
  scoped_refptr<AudioBuffer> GetNextBuffer();

 private:
  void AddOutputBuffer(const scoped_refptr<AudioBuffer>& buffer);

  AudioTimestampHelper output_timestamp_helper_;

  
  
  
  
  int min_gap_size_;

  std::deque<scoped_refptr<AudioBuffer> > output_buffers_;
  bool received_end_of_stream_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(AudioSplicer);
};

}  

#endif
