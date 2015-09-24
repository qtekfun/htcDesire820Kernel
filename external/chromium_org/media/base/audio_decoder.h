// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_AUDIO_DECODER_H_
#define MEDIA_BASE_AUDIO_DECODER_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "media/base/channel_layout.h"
#include "media/base/pipeline_status.h"
#include "media/base/media_export.h"

namespace media {

class AudioBuffer;
class DemuxerStream;

class MEDIA_EXPORT AudioDecoder {
 public:
  
  enum Status {
    kOk,
    kAborted,
    kDecodeError,
  };

  AudioDecoder();
  virtual ~AudioDecoder();

  
  
  
  virtual void Initialize(DemuxerStream* stream,
                          const PipelineStatusCB& status_cb,
                          const StatisticsCB& statistics_cb) = 0;

  
  
  
  
  
  
  
  
  
  
  typedef base::Callback<void(Status, const scoped_refptr<AudioBuffer>&)>
      ReadCB;
  virtual void Read(const ReadCB& read_cb) = 0;

  
  virtual void Reset(const base::Closure& closure) = 0;

  
  virtual int bits_per_channel() = 0;
  virtual ChannelLayout channel_layout() = 0;
  virtual int samples_per_second() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(AudioDecoder);
};

}  

#endif  
