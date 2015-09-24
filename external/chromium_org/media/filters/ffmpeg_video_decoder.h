// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FILTERS_FFMPEG_VIDEO_DECODER_H_
#define MEDIA_FILTERS_FFMPEG_VIDEO_DECODER_H_

#include <list>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "media/base/video_decoder.h"
#include "media/base/video_decoder_config.h"
#include "media/base/video_frame_pool.h"

struct AVCodecContext;
struct AVFrame;

namespace base {
class MessageLoopProxy;
}

namespace media {

class DecoderBuffer;
class ScopedPtrAVFreeContext;
class ScopedPtrAVFreeFrame;

class MEDIA_EXPORT FFmpegVideoDecoder : public VideoDecoder {
 public:
  explicit FFmpegVideoDecoder(
      const scoped_refptr<base::MessageLoopProxy>& message_loop);
  virtual ~FFmpegVideoDecoder();

  
  virtual void Initialize(const VideoDecoderConfig& config,
                          const PipelineStatusCB& status_cb) OVERRIDE;
  virtual void Decode(const scoped_refptr<DecoderBuffer>& buffer,
                      const DecodeCB& decode_cb) OVERRIDE;
  virtual void Reset(const base::Closure& closure) OVERRIDE;
  virtual void Stop(const base::Closure& closure) OVERRIDE;

  
  
  
  int GetVideoBuffer(AVCodecContext *codec_context, AVFrame* frame);

 private:
  enum DecoderState {
    kUninitialized,
    kNormal,
    kFlushCodec,
    kDecodeFinished,
    kError
  };

  
  void DecodeBuffer(const scoped_refptr<DecoderBuffer>& buffer);
  bool FFmpegDecode(const scoped_refptr<DecoderBuffer>& buffer,
                    scoped_refptr<VideoFrame>* video_frame);

  
  
  bool ConfigureDecoder();

  
  
  void ReleaseFFmpegResources();

  
  void DoReset();

  scoped_refptr<base::MessageLoopProxy> message_loop_;
  base::WeakPtrFactory<FFmpegVideoDecoder> weak_factory_;
  base::WeakPtr<FFmpegVideoDecoder> weak_this_;

  DecoderState state_;

  DecodeCB decode_cb_;
  base::Closure reset_cb_;

  
  scoped_ptr_malloc<AVCodecContext, ScopedPtrAVFreeContext> codec_context_;
  scoped_ptr_malloc<AVFrame, ScopedPtrAVFreeFrame> av_frame_;

  VideoDecoderConfig config_;

  VideoFramePool frame_pool_;

  DISALLOW_COPY_AND_ASSIGN(FFmpegVideoDecoder);
};

}  

#endif  
