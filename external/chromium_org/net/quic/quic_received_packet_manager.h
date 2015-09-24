// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_RECEIVED_PACKET_MANAGER_H_
#define NET_QUIC_QUIC_RECEIVED_PACKET_MANAGER_H_

#include "net/quic/congestion_control/receive_algorithm_interface.h"
#include "net/quic/quic_framer.h"
#include "net/quic/quic_protocol.h"

namespace net {

namespace test {
class QuicConnectionPeer;
class QuicReceivedPacketManagerPeer;
}  

class NET_EXPORT_PRIVATE QuicReceivedPacketManager :
    public QuicReceivedEntropyHashCalculatorInterface {
 public:
  explicit QuicReceivedPacketManager(CongestionFeedbackType congestion_type);
  virtual ~QuicReceivedPacketManager();

  
  
  
  
  
  
  void RecordPacketReceived(QuicByteCount bytes,
                            const QuicPacketHeader& header,
                            QuicTime receipt_time,
                            bool revived);

  
  bool IsMissing(QuicPacketSequenceNumber sequence_number);

  
  bool IsAwaitingPacket(QuicPacketSequenceNumber sequence_number);

  
  void UpdateReceivedPacketInfo(ReceivedPacketInfo* received_info,
                                QuicTime approximate_now);

  
  
  
  
  virtual bool GenerateCongestionFeedback(
      QuicCongestionFeedbackFrame* feedback);

  
  
  
  virtual QuicPacketEntropyHash EntropyHash(
      QuicPacketSequenceNumber sequence_number) const OVERRIDE;

  
  
  
  void UpdatePacketInformationReceivedByPeer(const QuicAckFrame& incoming_ack);
  
  void UpdatePacketInformationSentByPeer(const QuicAckFrame& incoming_ack);

  
  bool HasMissingPackets();

  
  
  bool HasNewMissingPackets();

  QuicPacketSequenceNumber peer_largest_observed_packet() {
    return peer_largest_observed_packet_;
  }

  QuicPacketSequenceNumber least_packet_awaited_by_peer() {
    return least_packet_awaited_by_peer_;
  }

  QuicPacketSequenceNumber peer_least_packet_awaiting_ack() {
    return peer_least_packet_awaiting_ack_;
  }

 private:
  friend class test::QuicConnectionPeer;
  friend class test::QuicReceivedPacketManagerPeer;

  typedef std::map<QuicPacketSequenceNumber,
                   QuicPacketEntropyHash> ReceivedEntropyMap;

  
  void RecordPacketEntropyHash(QuicPacketSequenceNumber sequence_number,
                               QuicPacketEntropyHash entropy_hash);

  
  
  
  void RecalculateEntropyHash(QuicPacketSequenceNumber peer_least_unacked,
                              QuicPacketEntropyHash entropy_hash);

  
  
  
  
  bool DontWaitForPacketsBefore(QuicPacketSequenceNumber least_unacked);

  
  
  
  
  
  
  ReceivedEntropyMap packets_entropy_;

  
  QuicPacketEntropyHash packets_entropy_hash_;

  
  
  QuicPacketSequenceNumber largest_sequence_number_;


  
  
  
  QuicPacketSequenceNumber peer_largest_observed_packet_;
  
  QuicPacketSequenceNumber least_packet_awaited_by_peer_;
  
  
  QuicPacketSequenceNumber peer_least_packet_awaiting_ack_;

  
  ReceivedPacketInfo received_info_;

  
  
  
  QuicTime time_largest_observed_;

  scoped_ptr<ReceiveAlgorithmInterface> receive_algorithm_;
};

}  

#endif  
