// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_CONGESTION_CONTROL_TCP_CUBIC_SENDER_H_
#define NET_QUIC_CONGESTION_CONTROL_TCP_CUBIC_SENDER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "net/base/net_export.h"
#include "net/quic/congestion_control/cubic.h"
#include "net/quic/congestion_control/hybrid_slow_start.h"
#include "net/quic/congestion_control/send_algorithm_interface.h"
#include "net/quic/quic_bandwidth.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_time.h"

namespace net {

const QuicByteCount kDefaultTCPMSS = 1460;

namespace test {
class TcpCubicSenderPeer;
}  

class NET_EXPORT_PRIVATE TcpCubicSender : public SendAlgorithmInterface {
 public:
  
  TcpCubicSender(const QuicClock* clock,
                 bool reno,
                 QuicTcpCongestionWindow max_tcp_congestion_window);
  virtual ~TcpCubicSender();

  
  virtual void SetFromConfig(const QuicConfig& config, bool is_server) OVERRIDE;
  virtual void SetMaxPacketSize(QuicByteCount max_packet_size) OVERRIDE;
  virtual void OnIncomingQuicCongestionFeedbackFrame(
      const QuicCongestionFeedbackFrame& feedback,
      QuicTime feedback_receive_time,
      const SentPacketsMap& sent_packets) OVERRIDE;
  virtual void OnPacketAcked(QuicPacketSequenceNumber acked_sequence_number,
                             QuicByteCount acked_bytes,
                             QuicTime::Delta rtt) OVERRIDE;
  virtual void OnPacketLost(QuicPacketSequenceNumber largest_loss,
                            QuicTime ack_receive_time) OVERRIDE;
  virtual bool OnPacketSent(QuicTime sent_time,
                            QuicPacketSequenceNumber sequence_number,
                            QuicByteCount bytes,
                            TransmissionType transmission_type,
                            HasRetransmittableData is_retransmittable) OVERRIDE;
  virtual void OnRetransmissionTimeout() OVERRIDE;
  virtual void OnPacketAbandoned(QuicPacketSequenceNumber sequence_number,
                                 QuicByteCount abandoned_bytes) OVERRIDE;
  virtual QuicTime::Delta TimeUntilSend(
      QuicTime now,
      TransmissionType transmission_type,
      HasRetransmittableData has_retransmittable_data,
      IsHandshake handshake) OVERRIDE;
  virtual QuicBandwidth BandwidthEstimate() const OVERRIDE;
  virtual QuicTime::Delta SmoothedRtt() const OVERRIDE;
  virtual QuicTime::Delta RetransmissionDelay() const OVERRIDE;
  virtual QuicByteCount GetCongestionWindow() const OVERRIDE;
  

 private:
  friend class test::TcpCubicSenderPeer;

  QuicByteCount AvailableSendWindow();
  QuicByteCount SendWindow();
  void Reset();
  void AckAccounting(QuicTime::Delta rtt);
  void CongestionAvoidance(QuicPacketSequenceNumber ack);
  bool IsCwndLimited() const;
  void OnTimeOut();

  HybridSlowStart hybrid_slow_start_;
  Cubic cubic_;

  
  const bool reno_;

  
  int64 congestion_window_count_;

  
  QuicByteCount receive_window_;

  
  int last_received_accumulated_number_of_lost_packets_;

  
  QuicByteCount bytes_in_flight_;

  
  bool update_end_sequence_number_;
  QuicPacketSequenceNumber end_sequence_number_;

  
  QuicPacketSequenceNumber largest_sent_sequence_number_;

  
  QuicPacketSequenceNumber largest_acked_sequence_number_;

  
  QuicPacketSequenceNumber largest_sent_at_last_cutback_;

  
  QuicTcpCongestionWindow congestion_window_;

  
  QuicTcpCongestionWindow slowstart_threshold_;

  
  QuicTcpCongestionWindow max_tcp_congestion_window_;

  
  QuicTime::Delta delay_min_;

  
  QuicTime::Delta smoothed_rtt_;

  
  
  
  QuicTime::Delta mean_deviation_;

  DISALLOW_COPY_AND_ASSIGN(TcpCubicSender);
};

}  

#endif  
