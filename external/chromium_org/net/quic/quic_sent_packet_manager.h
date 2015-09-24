// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_SENT_PACKET_MANAGER_H_
#define NET_QUIC_QUIC_SENT_PACKET_MANAGER_H_

#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <utility>
#include <vector>

#include "base/containers/hash_tables.h"
#include "net/base/linked_hash_map.h"
#include "net/quic/congestion_control/send_algorithm_interface.h"
#include "net/quic/quic_ack_notifier_manager.h"
#include "net/quic/quic_protocol.h"

NET_EXPORT_PRIVATE extern bool FLAGS_track_retransmission_history;
NET_EXPORT_PRIVATE extern bool FLAGS_limit_rto_increase_for_tests;
NET_EXPORT_PRIVATE extern bool FLAGS_enable_quic_pacing;

namespace net {

namespace test {
class QuicConnectionPeer;
class QuicSentPacketManagerPeer;
}  

class QuicClock;
class QuicConfig;

class NET_EXPORT_PRIVATE QuicSentPacketManager {
 public:
  
  struct PendingRetransmission {
    PendingRetransmission(QuicPacketSequenceNumber sequence_number,
                          TransmissionType transmission_type,
                          const RetransmittableFrames& retransmittable_frames,
                          QuicSequenceNumberLength sequence_number_length)
            : sequence_number(sequence_number),
              transmission_type(transmission_type),
              retransmittable_frames(retransmittable_frames),
              sequence_number_length(sequence_number_length) {
        }

        QuicPacketSequenceNumber sequence_number;
        TransmissionType transmission_type;
        const RetransmittableFrames& retransmittable_frames;
        QuicSequenceNumberLength sequence_number_length;
  };

  
  class NET_EXPORT_PRIVATE HelperInterface {
   public:
    virtual ~HelperInterface();

    
    virtual QuicPacketSequenceNumber GetNextPacketSequenceNumber() = 0;
  };

  QuicSentPacketManager(bool is_server,
                        HelperInterface* helper,
                        const QuicClock* clock,
                        CongestionFeedbackType congestion_type);
  virtual ~QuicSentPacketManager();

  virtual void SetFromConfig(const QuicConfig& config);

  virtual void SetMaxPacketSize(QuicByteCount max_packet_size);

  
  
  void OnSerializedPacket(const SerializedPacket& serialized_packet);

  
  
  
  void OnRetransmittedPacket(QuicPacketSequenceNumber old_sequence_number,
                             QuicPacketSequenceNumber new_sequence_number);

  
  
  bool OnIncomingAck(const ReceivedPacketInfo& received_info,
                     QuicTime ack_receive_time);

  
  
  
  void DiscardUnackedPacket(QuicPacketSequenceNumber sequence_number);

  
  bool IsUnacked(QuicPacketSequenceNumber sequence_number) const;

  
  void RetransmitUnackedPackets(RetransmissionType retransmission_type);

  
  
  
  
  bool HasRetransmittableFrames(QuicPacketSequenceNumber sequence_number) const;

  
  bool HasPendingRetransmissions() const;

  
  PendingRetransmission NextPendingRetransmission();

  bool HasUnackedPackets() const;

  
  size_t GetNumRetransmittablePackets() const;

  
  
  
  
  QuicPacketSequenceNumber GetLeastUnackedSentPacket() const;

  
  
  SequenceNumberSet GetUnackedPackets() const;

  
  bool IsPreviousTransmission(QuicPacketSequenceNumber sequence_number) const;

  
  
  
  
  
  virtual SequenceNumberSet OnIncomingAckFrame(
      const ReceivedPacketInfo& received_info,
      const QuicTime& ack_receive_time);

  
  virtual void OnIncomingQuicCongestionFeedbackFrame(
      const QuicCongestionFeedbackFrame& frame,
      const QuicTime& feedback_receive_time);

  
  
  virtual void OnPacketSent(QuicPacketSequenceNumber sequence_number,
                            QuicTime sent_time,
                            QuicByteCount bytes,
                            TransmissionType transmission_type,
                            HasRetransmittableData has_retransmittable_data);

  
  virtual void OnRetransmissionTimeout();

  
  
  virtual void OnPacketAbandoned(QuicPacketSequenceNumber sequence_number);

  
  
  
  
  
  virtual QuicTime::Delta TimeUntilSend(QuicTime now,
                                        TransmissionType transmission_type,
                                        HasRetransmittableData retransmittable,
                                        IsHandshake handshake);

  
  const QuicTime::Delta DelayedAckTime();

  
  const QuicTime::Delta GetRetransmissionDelay() const;

  
  const QuicTime::Delta SmoothedRtt() const;

  
  QuicBandwidth BandwidthEstimate() const;

  
  
  
  QuicByteCount GetCongestionWindow() const;

  
  
  void MaybeEnablePacing();

  bool using_pacing() const { return using_pacing_; }


 private:
  friend class test::QuicConnectionPeer;
  friend class test::QuicSentPacketManagerPeer;

  struct TransmissionInfo {
    TransmissionInfo()
        : retransmittable_frames(NULL),
          sequence_number_length(PACKET_1BYTE_SEQUENCE_NUMBER),
          sent_time(QuicTime::Zero()),
          previous_transmissions(NULL) { }
    TransmissionInfo(RetransmittableFrames* retransmittable_frames,
                     QuicSequenceNumberLength sequence_number_length)
        : retransmittable_frames(retransmittable_frames),
          sequence_number_length(sequence_number_length),
          sent_time(QuicTime::Zero()),
          previous_transmissions(NULL) {
    }

    RetransmittableFrames* retransmittable_frames;
    QuicSequenceNumberLength sequence_number_length;
    
    QuicTime sent_time;
    
    
    SequenceNumberSet* previous_transmissions;
  };

  typedef linked_hash_map<QuicPacketSequenceNumber,
                          TransmissionInfo> UnackedPacketMap;
  typedef linked_hash_map<QuicPacketSequenceNumber,
                          TransmissionType> PendingRetransmissionMap;
  typedef base::hash_map<QuicPacketSequenceNumber, SequenceNumberSet*>
                         PreviousTransmissionMap;

  
  void HandleAckForSentPackets(const ReceivedPacketInfo& received_info);

  
  void MaybeUpdateRTT(const ReceivedPacketInfo& received_info,
                      const QuicTime& ack_receive_time);

  
  
  UnackedPacketMap::iterator MarkPacketReceivedByPeer(
      QuicPacketSequenceNumber sequence_number);

  
  
  void DiscardPacket(QuicPacketSequenceNumber sequence_number);

  
  
  
  bool MarkForRetransmission(QuicPacketSequenceNumber sequence_number,
                             TransmissionType transmission_type);

  
  
  QuicSequenceNumberLength GetSequenceNumberLength(
      QuicPacketSequenceNumber sequence_number) const;

  
  
  void ClearPreviousRetransmissions(size_t num_to_clear);

  void CleanupPacketHistory();

  
  
  
  
  
  
  
  
  UnackedPacketMap unacked_packets_;

  
  PendingRetransmissionMap pending_retransmissions_;

  
  bool is_server_;

  HelperInterface* helper_;

  
  
  
  AckNotifierManager ack_notifier_manager_;

  const QuicClock* clock_;
  scoped_ptr<SendAlgorithmInterface> send_algorithm_;
  
  
  SendAlgorithmInterface::SentPacketsMap packet_history_map_;
  
  SequenceNumberSet pending_packets_;
  QuicTime::Delta rtt_sample_;  
  
  size_t consecutive_rto_count_;
  bool using_pacing_;

  DISALLOW_COPY_AND_ASSIGN(QuicSentPacketManager);
};

}  

#endif  
