// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_BASE_VIDEO_DECODER_H_
#define MEDIA_BASE_VIDEO_DECODER_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "media/base/media_export.h"
#include "media/base/pipeline_status.h"
#include "ui/gfx/size.h"

namespace media {

class DecoderBuffer;
class VideoDecoderConfig;
class VideoFrame;

class MEDIA_EXPORT VideoDecoder {
 public:
  
  enum Status {
    kOk,  
    kNotEnoughData,  
    kDecodeError,  
    kDecryptError  
  };

  VideoDecoder();
  virtual ~VideoDecoder();

  
  
  
  
  
  
  
  
  
  virtual void Initialize(const VideoDecoderConfig& config,
                          const PipelineStatusCB& status_cb) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  typedef base::Callback<void(Status,
                              const scoped_refptr<VideoFrame>&)> DecodeCB;
  virtual void Decode(const scoped_refptr<DecoderBuffer>& buffer,
                      const DecodeCB& decode_cb) = 0;

  
  
  
  
  virtual void Reset(const base::Closure& closure) = 0;

  
  
  
  
  
  virtual void Stop(const base::Closure& closure) = 0;

  
  
  
  virtual bool HasAlpha() const;

  
  virtual bool NeedsBitstreamConversion() const;

  
  
  
  
  virtual bool CanReadWithoutStalling() const;

 private:
  DISALLOW_COPY_AND_ASSIGN(VideoDecoder);
};

}  

#endif  
