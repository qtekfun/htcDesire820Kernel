// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef MEDIA_CAST_NET_RTP_SENDER_RTP_SENDER_H_
#define MEDIA_CAST_NET_RTP_SENDER_RTP_SENDER_H_

#include <map>
#include <set>

#include "base/memory/scoped_ptr.h"
#include "base/time/tick_clock.h"
#include "base/time/time.h"
#include "media/cast/cast_config.h"
#include "media/cast/cast_environment.h"
#include "media/cast/net/rtp_sender/packet_storage/packet_storage.h"
#include "media/cast/net/rtp_sender/rtp_packetizer/rtp_packetizer.h"
#include "media/cast/net/rtp_sender/rtp_packetizer/rtp_packetizer_config.h"

namespace media {
namespace cast {

class PacedPacketSender;
struct RtcpSenderInfo;

class RtpSender {
 public:
  RtpSender(scoped_refptr<CastEnvironment> cast_environment,
            const AudioSenderConfig* audio_config,
            const VideoSenderConfig* video_config,
            PacedPacketSender* transport);

  ~RtpSender();

  
  void IncomingEncodedVideoFrame(const EncodedVideoFrame* video_frame,
                                 const base::TimeTicks& capture_time);

  
  void IncomingEncodedAudioFrame(const EncodedAudioFrame* audio_frame,
                                 const base::TimeTicks& recorded_time);

  void ResendPackets(const MissingFramesAndPacketsMap& missing_packets);

  void RtpStatistics(const base::TimeTicks& now, RtcpSenderInfo* sender_info);

 private:
  void UpdateSequenceNumber(std::vector<uint8>* packet);

  scoped_refptr<CastEnvironment> cast_environment_;
  RtpPacketizerConfig config_;
  scoped_ptr<RtpPacketizer> packetizer_;
  scoped_ptr<PacketStorage> storage_;
  PacedPacketSender* transport_;
};

}  
}  

#endif  
