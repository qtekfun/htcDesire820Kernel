// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_CAST_RECEIVER_IMPL_H_
#define MEDIA_CAST_CAST_RECEIVER_IMPL_H_

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "media/cast/audio_receiver/audio_receiver.h"
#include "media/cast/cast_config.h"
#include "media/cast/cast_environment.h"
#include "media/cast/cast_receiver.h"
#include "media/cast/net/pacing/paced_sender.h"
#include "media/cast/video_receiver/video_receiver.h"

namespace media {
namespace cast {

class CastReceiverImpl : public CastReceiver {
 public:
  CastReceiverImpl(scoped_refptr<CastEnvironment> cast_environment,
                   const AudioReceiverConfig& audio_config,
                   const VideoReceiverConfig& video_config,
                   PacketSender* const packet_sender);

  virtual ~CastReceiverImpl();

  
  
  virtual scoped_refptr<PacketReceiver> packet_receiver() OVERRIDE;

  
  virtual scoped_refptr<FrameReceiver> frame_receiver() OVERRIDE;

 private:
  PacedSender pacer_;
  AudioReceiver audio_receiver_;
  VideoReceiver video_receiver_;
  scoped_refptr<FrameReceiver> frame_receiver_;
  scoped_refptr<PacketReceiver> packet_receiver_;
};

}  
}  

#endif  