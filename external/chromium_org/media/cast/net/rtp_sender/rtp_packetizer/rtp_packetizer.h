// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_CAST_NET_RTP_SENDER_RTP_PACKETIZER_RTP_PACKETIZER_H_
#define MEDIA_CAST_NET_RTP_SENDER_RTP_PACKETIZER_RTP_PACKETIZER_H_

#include <cmath>
#include <list>
#include <map>

#include "base/time/time.h"
#include "media/cast/net/rtp_sender/packet_storage/packet_storage.h"
#include "media/cast/net/rtp_sender/rtp_packetizer/rtp_packetizer_config.h"

namespace media {
namespace cast {

class PacedPacketSender;

class RtpPacketizer {
 public:
  RtpPacketizer(PacedPacketSender* transport,
                PacketStorage* packet_storage,
                RtpPacketizerConfig rtp_packetizer_config);
  ~RtpPacketizer();

  
  void IncomingEncodedVideoFrame(const EncodedVideoFrame* video_frame,
                                 const base::TimeTicks& capture_time);

  
  void IncomingEncodedAudioFrame(const EncodedAudioFrame* audio_frame,
                                 const base::TimeTicks& recorded_time);

  bool LastSentTimestamp(base::TimeTicks* time_sent,
                         uint32* rtp_timestamp) const;

  
  
  uint16 NextSequenceNumber();

  int send_packets_count() { return send_packets_count_; }

  size_t send_octet_count() { return send_octet_count_; }

 private:
  void Cast(bool is_key, uint32 frame_id, uint32 reference_frame_id,
            uint32 timestamp, const std::string& data);

  void BuildCommonRTPheader(std::vector<uint8>* packet, bool marker_bit,
      uint32 time_stamp);

  RtpPacketizerConfig config_;
  PacedPacketSender* transport_;
  PacketStorage* packet_storage_;

  base::TimeTicks time_last_sent_rtp_timestamp_;
  uint16 sequence_number_;
  uint32 rtp_timestamp_;
  uint16 packet_id_;

  int send_packets_count_;
  size_t send_octet_count_;
};

}  
}  

#endif  
