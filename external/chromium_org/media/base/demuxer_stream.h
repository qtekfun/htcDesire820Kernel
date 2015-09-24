// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_DEMUXER_STREAM_H_
#define MEDIA_BASE_DEMUXER_STREAM_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "media/base/media_export.h"

namespace media {

class AudioDecoderConfig;
class DecoderBuffer;
class VideoDecoderConfig;

class MEDIA_EXPORT DemuxerStream {
 public:
  enum Type {
    UNKNOWN,
    AUDIO,
    VIDEO,
    TEXT,
    NUM_TYPES,  
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  enum Status {
    kOk,
    kAborted,
    kConfigChanged,
  };

  
  
  
  
  
  typedef base::Callback<void(Status,
                              const scoped_refptr<DecoderBuffer>&)>ReadCB;
  virtual void Read(const ReadCB& read_cb) = 0;

  
  
  virtual AudioDecoderConfig audio_decoder_config() = 0;

  
  
  virtual VideoDecoderConfig video_decoder_config() = 0;

  
  virtual Type type() = 0;

  virtual void EnableBitstreamConverter() = 0;

 protected:
  
  virtual ~DemuxerStream();
};

}  

#endif  
