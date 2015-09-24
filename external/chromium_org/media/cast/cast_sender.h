// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_CAST_SENDER_H_
#define MEDIA_CAST_CAST_SENDER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/time/tick_clock.h"
#include "base/time/time.h"
#include "media/cast/cast_config.h"
#include "media/cast/cast_environment.h"

namespace media {
class AudioBus;
class VideoFrame;
}

namespace media {
namespace cast {

class FrameInput : public base::RefCountedThreadSafe<FrameInput> {
 public:
  
  
  
  
  virtual void InsertRawVideoFrame(
      const scoped_refptr<media::VideoFrame>& video_frame,
      const base::TimeTicks& capture_time) = 0;

  
  
  
  
  virtual void InsertCodedVideoFrame(const EncodedVideoFrame* video_frame,
                                     const base::TimeTicks& capture_time,
                                     const base::Closure callback) = 0;

  
  
  
  
  virtual void InsertAudio(const AudioBus* audio_bus,
                           const base::TimeTicks& recorded_time,
                           const base::Closure& done_callback) = 0;

  
  
  
  
  virtual void InsertCodedAudioFrame(const EncodedAudioFrame* audio_frame,
                                     const base::TimeTicks& recorded_time,
                                     const base::Closure callback) = 0;

 protected:
  virtual ~FrameInput() {}

 private:
  friend class base::RefCountedThreadSafe<FrameInput>;
};

class CastSender {
 public:
  static CastSender* CreateCastSender(
      scoped_refptr<CastEnvironment> cast_environment,
      const AudioSenderConfig& audio_config,
      const VideoSenderConfig& video_config,
      VideoEncoderController* const video_encoder_controller,
      PacketSender* const packet_sender);

  virtual ~CastSender() {}

  
  
  
  virtual scoped_refptr<FrameInput> frame_input() = 0;

  
  
  virtual scoped_refptr<PacketReceiver> packet_receiver() = 0;
};

}  
}  

#endif  
