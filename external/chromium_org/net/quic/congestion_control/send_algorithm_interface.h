// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CONGESTION_CONTROL_SEND_ALGORITHM_INTERFACE_H_
#define NET_QUIC_CONGESTION_CONTROL_SEND_ALGORITHM_INTERFACE_H_

#include <algorithm>
#include <map>

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/quic/quic_bandwidth.h"
#include "net/quic/quic_clock.h"
#include "net/quic/quic_config.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_time.h"

namespace net {

class NET_EXPORT_PRIVATE SendAlgorithmInterface {
 public:
  class SentPacket {
   public:
    SentPacket(QuicByteCount bytes,
               QuicTime timestamp,
               HasRetransmittableData has_retransmittable_data)
        : bytes_sent_(bytes),
          send_timestamp_(timestamp),
          has_retransmittable_data_(has_retransmittable_data),
          nack_count_(0) {
    }
    QuicByteCount bytes_sent() const { return bytes_sent_; }
    const QuicTime& send_timestamp() const { return send_timestamp_; }
    HasRetransmittableData has_retransmittable_data() const {
      return has_retransmittable_data_;
    }
    size_t nack_count() const { return nack_count_; }

    void Nack(size_t min_nacks) {
      nack_count_ = std::max(min_nacks, nack_count_ + 1);
    }

   private:
    QuicByteCount bytes_sent_;
    QuicTime send_timestamp_;
    HasRetransmittableData has_retransmittable_data_;
    size_t nack_count_;
  };

  typedef std::map<QuicPacketSequenceNumber, SentPacket*> SentPacketsMap;

  static SendAlgorithmInterface* Create(const QuicClock* clock,
                                        CongestionFeedbackType type);

  virtual ~SendAlgorithmInterface() {}

  virtual void SetFromConfig(const QuicConfig& config, bool is_server) = 0;

  
  virtual void SetMaxPacketSize(QuicByteCount max_packet_size) = 0;

  
  virtual void OnIncomingQuicCongestionFeedbackFrame(
      const QuicCongestionFeedbackFrame& feedback,
      QuicTime feedback_receive_time,
      const SentPacketsMap& sent_packets) = 0;

  
  virtual void OnPacketAcked(QuicPacketSequenceNumber acked_sequence_number,
                             QuicByteCount acked_bytes,
                             QuicTime::Delta rtt) = 0;

  
  
  virtual void OnPacketLost(QuicPacketSequenceNumber sequence_number,
                            QuicTime ack_receive_time) = 0;

  
  
  
  
  
  virtual bool OnPacketSent(QuicTime sent_time,
                            QuicPacketSequenceNumber sequence_number,
                            QuicByteCount bytes,
                            TransmissionType transmission_type,
                            HasRetransmittableData is_retransmittable) = 0;

  
  virtual void OnRetransmissionTimeout() = 0;

  
  virtual void OnPacketAbandoned(QuicPacketSequenceNumber sequence_number,
                                QuicByteCount abandoned_bytes) = 0;

  
  virtual QuicTime::Delta TimeUntilSend(
      QuicTime now,
      TransmissionType transmission_type,
      HasRetransmittableData has_retransmittable_data,
      IsHandshake handshake) = 0;

  
  
  virtual QuicBandwidth BandwidthEstimate() const = 0;

  
  virtual QuicTime::Delta SmoothedRtt() const = 0;

  
  
  
  virtual QuicTime::Delta RetransmissionDelay() const = 0;

  
  
  
  virtual QuicByteCount GetCongestionWindow() const = 0;
};

}  

#endif  
