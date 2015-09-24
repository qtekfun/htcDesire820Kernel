// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_AUDIO_FILE_READER_H_
#define MEDIA_FILTERS_AUDIO_FILE_READER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "media/base/media_export.h"

struct AVCodecContext;

namespace base { class TimeDelta; }

namespace media {

class AudioBus;
class FFmpegGlue;
class FFmpegURLProtocol;

class MEDIA_EXPORT AudioFileReader {
 public:
  
  
  
  explicit AudioFileReader(FFmpegURLProtocol* protocol);
  virtual ~AudioFileReader();

  
  
  
  bool Open();
  void Close();

  
  
  
  
  
  
  
  int Read(AudioBus* audio_bus);

  
  int channels() const { return channels_; }
  int sample_rate() const { return sample_rate_; }

  
  
  
  
  base::TimeDelta duration() const;
  int64 number_of_frames() const;

 private:
  scoped_ptr<FFmpegGlue> glue_;
  AVCodecContext* codec_context_;
  int stream_index_;
  FFmpegURLProtocol* protocol_;
  int channels_;
  int sample_rate_;

  
  int av_sample_format_;

  DISALLOW_COPY_AND_ASSIGN(AudioFileReader);
};

}  

#endif  
