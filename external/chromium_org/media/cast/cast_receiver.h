// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_CAST_RECEIVER_H_
#define MEDIA_CAST_CAST_RECEIVER_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/time/time.h"
#include "media/cast/cast_config.h"
#include "media/cast/cast_environment.h"

namespace media {
class VideoFrame;
}

namespace media {
namespace cast {
typedef base::Callback<void(scoped_ptr<PcmAudioFrame>, const base::TimeTicks&)>
    AudioFrameDecodedCallback;

typedef base::Callback<void(scoped_ptr<EncodedAudioFrame>,
    const base::TimeTicks&)> AudioFrameEncodedCallback;

typedef base::Callback<void(const scoped_refptr<media::VideoFrame>& video_frame,
                            const base::TimeTicks&)>
    VideoFrameDecodedCallback;

typedef base::Callback<void(scoped_ptr<EncodedVideoFrame>,
    const base::TimeTicks&)> VideoFrameEncodedCallback;

class FrameReceiver : public base::RefCountedThreadSafe<FrameReceiver> {
 public:
  virtual void GetRawAudioFrame(int number_of_10ms_blocks,
                                int desired_frequency,
                                const AudioFrameDecodedCallback& callback) = 0;

  virtual void GetCodedAudioFrame(
      const AudioFrameEncodedCallback& callback) = 0;

  virtual void GetRawVideoFrame(const VideoFrameDecodedCallback& callback) = 0;

  virtual void GetEncodedVideoFrame(
    const VideoFrameEncodedCallback& callback) = 0;

 protected:
  virtual ~FrameReceiver() {}

 private:
  friend class base::RefCountedThreadSafe<FrameReceiver>;
};

class CastReceiver {
 public:
  static CastReceiver* CreateCastReceiver(
      scoped_refptr<CastEnvironment> cast_environment,
      const AudioReceiverConfig& audio_config,
      const VideoReceiverConfig& video_config,
      PacketSender* const packet_sender);

  
  
  virtual scoped_refptr<PacketReceiver> packet_receiver() = 0;

  
  virtual scoped_refptr<FrameReceiver> frame_receiver() = 0;

  virtual ~CastReceiver() {}
};

}  
}  

#endif  
