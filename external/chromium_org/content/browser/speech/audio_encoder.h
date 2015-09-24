// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_SPEECH_AUDIO_ENCODER_H_
#define CONTENT_BROWSER_SPEECH_AUDIO_ENCODER_H_

#include <list>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "content/browser/speech/audio_buffer.h"

namespace content{
class AudioChunk;

class AudioEncoder {
 public:
  enum Codec {
    CODEC_FLAC,
    CODEC_SPEEX,
  };

  static AudioEncoder* Create(Codec codec,
                              int sampling_rate,
                              int bits_per_sample);

  virtual ~AudioEncoder();

  
  
  
  virtual void Encode(const AudioChunk& raw_audio) = 0;

  
  virtual void Flush() = 0;

  
  scoped_refptr<AudioChunk> GetEncodedDataAndClear();

  const std::string& mime_type() { return mime_type_; }
  int bits_per_sample() { return bits_per_sample_; }

 protected:
  AudioEncoder(const std::string& mime_type, int bits_per_sample);
  AudioBuffer encoded_audio_buffer_;

 private:
  std::string mime_type_;
  int bits_per_sample_;

  DISALLOW_COPY_AND_ASSIGN(AudioEncoder);
};

}  

#endif  
