// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_FFMPEG_FFMPEG_COMMON_H_
#define MEDIA_FFMPEG_FFMPEG_COMMON_H_

#include <cerrno>

#include "base/compiler_specific.h"
#include "base/time/time.h"
#include "media/base/audio_decoder_config.h"
#include "media/base/channel_layout.h"
#include "media/base/media_export.h"
#include "media/base/video_decoder_config.h"
#include "media/base/video_frame.h"

extern "C" {
MSVC_PUSH_DISABLE_WARNING(4244);
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libavutil/audioconvert.h>
#include <libavutil/avutil.h>
#include <libavutil/mathematics.h>
#include <libavutil/log.h>
#include <libavutil/imgutils.h>
MSVC_POP_WARNING();
}  

namespace media {

class AudioDecoderConfig;
class VideoDecoderConfig;

class ScopedPtrAVFree {
 public:
  inline void operator()(void* x) const {
    av_free(x);
  }
};

class ScopedPtrAVFreePacket {
 public:
  inline void operator()(void* x) const {
    AVPacket* packet = static_cast<AVPacket*>(x);
    av_free_packet(packet);
    delete packet;
  }
};

class ScopedPtrAVFreeContext {
 public:
  inline void operator()(void* x) const {
    AVCodecContext* codec_context = static_cast<AVCodecContext*>(x);
    av_free(codec_context->extradata);
    avcodec_close(codec_context);
    av_free(codec_context);
  }
};

class ScopedPtrAVFreeFrame {
 public:
  inline void operator()(void* x) const {
    AVFrame* frame = static_cast<AVFrame*>(x);
    avcodec_free_frame(&frame);
  }
};

MEDIA_EXPORT base::TimeDelta ConvertFromTimeBase(const AVRational& time_base,
                                                 int64 timestamp);

MEDIA_EXPORT int64 ConvertToTimeBase(const AVRational& time_base,
                                     const base::TimeDelta& timestamp);

void AVStreamToAudioDecoderConfig(
    const AVStream* stream,
    AudioDecoderConfig* config,
    bool record_stats);
void AudioDecoderConfigToAVCodecContext(
    const AudioDecoderConfig& config,
    AVCodecContext* codec_context);

void AVStreamToVideoDecoderConfig(
    const AVStream* stream,
    VideoDecoderConfig* config,
    bool record_stats);
void VideoDecoderConfigToAVCodecContext(
    const VideoDecoderConfig& config,
    AVCodecContext* codec_context);

ChannelLayout ChannelLayoutToChromeChannelLayout(int64_t layout,
                                                 int channels);

MEDIA_EXPORT SampleFormat
    AVSampleFormatToSampleFormat(AVSampleFormat sample_format);

VideoFrame::Format PixelFormatToVideoFormat(PixelFormat pixel_format);

PixelFormat VideoFormatToPixelFormat(VideoFrame::Format video_format);

}  

#endif  
