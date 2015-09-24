// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_CONNECTION_H_
#define NET_QUIC_QUIC_CONNECTION_H_

#include <stddef.h>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <string>
#include <vector>

#include "base/logging.h"
#include "net/base/iovec.h"
#include "net/base/ip_endpoint.h"
#include "net/quic/iovector.h"
#include "net/quic/quic_ack_notifier.h"
#include "net/quic/quic_ack_notifier_manager.h"
#include "net/quic/quic_alarm.h"
#include "net/quic/quic_blocked_writer_interface.h"
#include "net/quic/quic_connection_stats.h"
#include "net/quic/quic_packet_creator.h"
#include "net/quic/quic_packet_generator.h"
#include "net/quic/quic_packet_writer.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_received_packet_manager.h"
#include "net/quic/quic_sent_entropy_manager.h"
#include "net/quic/quic_sent_packet_manager.h"

NET_EXPORT_PRIVATE extern int FLAGS_fake_packet_loss_percentage;
NET_EXPORT_PRIVATE extern bool FLAGS_bundle_ack_with_outgoing_packet;

namespace net {

class QuicClock;
class QuicConfig;
class QuicConnection;
class QuicDecrypter;
class QuicEncrypter;
class QuicFecGroup;
class QuicRandom;

namespace test {
class QuicConnectionPeer;
}  

class NET_EXPORT_PRIVATE QuicConnectionVisitorInterface {
 public:
  virtual ~QuicConnectionVisitorInterface() {}

  
  
  
  
  virtual bool OnStreamFrames(const std::vector<QuicStreamFrame>& frames) = 0;

  
  virtual void OnRstStream(const QuicRstStreamFrame& frame) = 0;

  
  virtual void OnGoAway(const QuicGoAwayFrame& frame) = 0;

  
  
  virtual void OnConnectionClosed(QuicErrorCode error,
                                  bool from_peer) = 0;

  
  virtual void OnSuccessfulVersionNegotiation(const QuicVersion& version) = 0;

  
  virtual void OnConfigNegotiated() = 0;

  
  
  
  virtual bool OnCanWrite() = 0;

  
  virtual bool HasPendingHandshake() const = 0;
};

class NET_EXPORT_PRIVATE QuicConnectionDebugVisitorInterface
    : public QuicPacketGenerator::DebugDelegateInterface {
 public:
  virtual ~QuicConnectionDebugVisitorInterface() {}

  
  virtual void OnPacketSent(QuicPacketSequenceNumber sequence_number,
                            EncryptionLevel level,
                            const QuicEncryptedPacket& packet,
                            WriteResult result) = 0;

  
  
  virtual void OnPacketRetransmitted(
      QuicPacketSequenceNumber old_sequence_number,
      QuicPacketSequenceNumber new_sequence_number) = 0;

  
  
  virtual void OnPacketReceived(const IPEndPoint& self_address,
                                const IPEndPoint& peer_address,
                                const QuicEncryptedPacket& packet) = 0;

  
  
  virtual void OnProtocolVersionMismatch(QuicVersion version) = 0;

  
  virtual void OnPacketHeader(const QuicPacketHeader& header) = 0;

  
  virtual void OnStreamFrame(const QuicStreamFrame& frame) = 0;

  
  virtual void OnAckFrame(const QuicAckFrame& frame) = 0;

  
  virtual void OnCongestionFeedbackFrame(
      const QuicCongestionFeedbackFrame& frame) = 0;

  
  virtual void OnRstStreamFrame(const QuicRstStreamFrame& frame) = 0;

  
  virtual void OnConnectionCloseFrame(
      const QuicConnectionCloseFrame& frame) = 0;

  
  virtual void OnPublicResetPacket(const QuicPublicResetPacket& packet) = 0;

  
  virtual void OnVersionNegotiationPacket(
      const QuicVersionNegotiationPacket& packet) = 0;

  
  
  virtual void OnRevivedPacket(const QuicPacketHeader& revived_header,
                               base::StringPiece payload) = 0;
};

class NET_EXPORT_PRIVATE QuicConnectionHelperInterface {
 public:
  virtual ~QuicConnectionHelperInterface() {}

  
  virtual const QuicClock* GetClock() const = 0;

  
  virtual QuicRandom* GetRandomGenerator() = 0;

  
  
  
  virtual QuicAlarm* CreateAlarm(QuicAlarm::Delegate* delegate) = 0;
};

class NET_EXPORT_PRIVATE QuicConnection
    : public QuicFramerVisitorInterface,
      public QuicBlockedWriterInterface,
      public QuicPacketGenerator::DelegateInterface,
      public QuicSentPacketManager::HelperInterface {
 public:
  enum Force {
    NO_FORCE,
    FORCE
  };

  
  
  QuicConnection(QuicGuid guid,
                 IPEndPoint address,
                 QuicConnectionHelperInterface* helper,
                 QuicPacketWriter* writer,
                 bool is_server,
                 const QuicVersionVector& supported_versions);
  virtual ~QuicConnection();

  
  void SetFromConfig(const QuicConfig& config);

  
  
  
  
  
  
  // received for all the packets written in this call.
  
  QuicConsumedData SendStreamData(QuicStreamId id,
                                  const IOVector& data,
                                  QuicStreamOffset offset,
                                  bool fin,
                                  QuicAckNotifier::DelegateInterface* delegate);

  
  virtual void SendRstStream(QuicStreamId id,
                             QuicRstStreamErrorCode error);

  
  
  
  virtual void SendConnectionClosePacket(QuicErrorCode error,
                                         const std::string& details);

  
  
  virtual void SendConnectionClose(QuicErrorCode error);
  virtual void SendConnectionCloseWithDetails(QuicErrorCode error,
                                              const std::string& details);
  
  virtual void CloseConnection(QuicErrorCode error, bool from_peer) OVERRIDE;
  virtual void SendGoAway(QuicErrorCode error,
                          QuicStreamId last_good_stream_id,
                          const std::string& reason);

  
  const QuicConnectionStats& GetStats();

  
  
  
  virtual void ProcessUdpPacket(const IPEndPoint& self_address,
                                const IPEndPoint& peer_address,
                                const QuicEncryptedPacket& packet);

  
  
  
  virtual bool OnCanWrite() OVERRIDE;

  
  bool OnPacketSent(WriteResult result);

  
  
  bool WriteIfNotBlocked();

  
  
  
  bool ProcessValidatedPacket();

  
  QuicVersion version() const { return framer_.version(); }

  
  const QuicVersionVector& supported_versions() const {
    return framer_.supported_versions();
  }

  
  virtual void OnError(QuicFramer* framer) OVERRIDE;
  virtual bool OnProtocolVersionMismatch(QuicVersion received_version) OVERRIDE;
  virtual void OnPacket() OVERRIDE;
  virtual void OnPublicResetPacket(
      const QuicPublicResetPacket& packet) OVERRIDE;
  virtual void OnVersionNegotiationPacket(
      const QuicVersionNegotiationPacket& packet) OVERRIDE;
  virtual void OnRevivedPacket() OVERRIDE;
  virtual bool OnUnauthenticatedHeader(const QuicPacketHeader& header) OVERRIDE;
  virtual bool OnPacketHeader(const QuicPacketHeader& header) OVERRIDE;
  virtual void OnFecProtectedPayload(base::StringPiece payload) OVERRIDE;
  virtual bool OnStreamFrame(const QuicStreamFrame& frame) OVERRIDE;
  virtual bool OnAckFrame(const QuicAckFrame& frame) OVERRIDE;
  virtual bool OnCongestionFeedbackFrame(
      const QuicCongestionFeedbackFrame& frame) OVERRIDE;
  virtual bool OnRstStreamFrame(const QuicRstStreamFrame& frame) OVERRIDE;
  virtual bool OnConnectionCloseFrame(
      const QuicConnectionCloseFrame& frame) OVERRIDE;
  virtual bool OnGoAwayFrame(const QuicGoAwayFrame& frame) OVERRIDE;
  virtual void OnFecData(const QuicFecData& fec) OVERRIDE;
  virtual void OnPacketComplete() OVERRIDE;

  
  virtual bool ShouldGeneratePacket(TransmissionType transmission_type,
                                    HasRetransmittableData retransmittable,
                                    IsHandshake handshake) OVERRIDE;
  virtual QuicAckFrame* CreateAckFrame() OVERRIDE;
  virtual QuicCongestionFeedbackFrame* CreateFeedbackFrame() OVERRIDE;
  virtual bool OnSerializedPacket(const SerializedPacket& packet) OVERRIDE;

  
  virtual QuicPacketSequenceNumber GetNextPacketSequenceNumber() OVERRIDE;

  
  void set_visitor(QuicConnectionVisitorInterface* visitor) {
    visitor_ = visitor;
  }
  void set_debug_visitor(QuicConnectionDebugVisitorInterface* debug_visitor) {
    debug_visitor_ = debug_visitor;
    packet_generator_.set_debug_delegate(debug_visitor);
  }
  const IPEndPoint& self_address() const { return self_address_; }
  const IPEndPoint& peer_address() const { return peer_address_; }
  QuicGuid guid() const { return guid_; }
  const QuicClock* clock() const { return clock_; }
  QuicRandom* random_generator() const { return random_generator_; }

  QuicPacketCreator::Options* options() { return packet_creator_.options(); }

  bool connected() const { return connected_; }

  
  const QuicVersionVector& server_supported_versions() const {
    DCHECK(!is_server_);
    return server_supported_versions_;
  }

  size_t NumFecGroups() const { return group_map_.size(); }

  
  size_t NumQueuedPackets() const { return queued_packets_.size(); }

  QuicEncryptedPacket* ReleaseConnectionClosePacket() {
    return connection_close_packet_.release();
  }

  
  
  void Flush();

  
  bool HasQueuedData() const;

  
  
  void SetIdleNetworkTimeout(QuicTime::Delta timeout);
  
  
  
  
  void SetOverallConnectionTimeout(QuicTime::Delta timeout);

  
  
  bool CheckForTimeout();

  
  void SendAck();

  
  
  void OnRetransmissionTimeout();

  
  
  
  
  
  void RetransmitUnackedPackets(RetransmissionType retransmission_type);

  
  
  void SetEncrypter(EncryptionLevel level, QuicEncrypter* encrypter);
  const QuicEncrypter* encrypter(EncryptionLevel level) const;

  
  
  void SetDefaultEncryptionLevel(EncryptionLevel level);

  
  
  
  
  
  void SetDecrypter(QuicDecrypter* decrypter);

  
  
  
  
  
  void SetAlternativeDecrypter(QuicDecrypter* decrypter,
                               bool latch_once_used);

  const QuicDecrypter* decrypter() const;
  const QuicDecrypter* alternative_decrypter() const;

  bool is_server() const { return is_server_; }

  
  const QuicSentPacketManager& sent_packet_manager() const {
    return sent_packet_manager_;
  }

  bool CanWrite(TransmissionType transmission_type,
                HasRetransmittableData retransmittable,
                IsHandshake handshake);

 protected:
  
  
  
  
  
  
  
  
  
  virtual bool SendOrQueuePacket(EncryptionLevel level,
                                 const SerializedPacket& packet,
                                 TransmissionType transmission_type);

  
  
  
  
  
  
  
  
  bool WritePacket(EncryptionLevel level,
                   QuicPacketSequenceNumber sequence_number,
                   QuicPacket* packet,
                   TransmissionType transmission_type,
                   HasRetransmittableData retransmittable,
                   IsHandshake handshake,
                   Force force);

  
  bool ValidateAckFrame(const QuicAckFrame& incoming_ack);

  QuicConnectionHelperInterface* helper() { return helper_; }

  
  
  
  bool SelectMutualVersion(const QuicVersionVector& available_versions);

  QuicFramer framer_;

 private:
  
  
  
  
  
  
  class ScopedPacketBundler {
   public:
    
    
    
    
    ScopedPacketBundler(QuicConnection* connection, bool include_ack);
    ~ScopedPacketBundler();

   private:
    QuicConnection* connection_;
    bool already_in_batch_mode_;
  };

  friend class ScopedPacketBundler;
  friend class test::QuicConnectionPeer;

  // Packets which have not been written to the wire.
  
  struct QueuedPacket {
    QueuedPacket(QuicPacketSequenceNumber sequence_number,
                 QuicPacket* packet,
                 EncryptionLevel level,
                 TransmissionType transmission_type,
                 HasRetransmittableData retransmittable,
                 IsHandshake handshake,
                 Force forced)
        : sequence_number(sequence_number),
          packet(packet),
          encryption_level(level),
          transmission_type(transmission_type),
          retransmittable(retransmittable),
          handshake(handshake),
          forced(forced) {
    }

    QuicPacketSequenceNumber sequence_number;
    QuicPacket* packet;
    const EncryptionLevel encryption_level;
    TransmissionType transmission_type;
    HasRetransmittableData retransmittable;
    IsHandshake handshake;
    Force forced;
  };

  struct RetransmissionInfo {
    RetransmissionInfo(QuicPacketSequenceNumber sequence_number,
                       QuicSequenceNumberLength sequence_number_length,
                       QuicTime sent_time)
        : sequence_number(sequence_number),
          sequence_number_length(sequence_number_length),
          sent_time(sent_time),
          number_nacks(0),
          number_retransmissions(0) {
    }

    QuicPacketSequenceNumber sequence_number;
    QuicSequenceNumberLength sequence_number_length;
    QuicTime sent_time;
    size_t number_nacks;
    size_t number_retransmissions;
  };

  struct RetransmissionTime {
    RetransmissionTime(QuicPacketSequenceNumber sequence_number,
                       const QuicTime& scheduled_time,
                       bool for_fec)
        : sequence_number(sequence_number),
          scheduled_time(scheduled_time),
          for_fec(for_fec) { }

    QuicPacketSequenceNumber sequence_number;
    QuicTime scheduled_time;
    bool for_fec;
  };

  struct PendingWrite {
    PendingWrite(QuicPacketSequenceNumber sequence_number,
                 TransmissionType transmission_type,
                 HasRetransmittableData retransmittable,
                 EncryptionLevel level,
                 bool is_fec_packet,
                 size_t length)
        : sequence_number(sequence_number),
          transmission_type(transmission_type),
          retransmittable(retransmittable),
          level(level),
          is_fec_packet(is_fec_packet),
          length(length) { }

    QuicPacketSequenceNumber sequence_number;
    TransmissionType transmission_type;
    HasRetransmittableData retransmittable;
    EncryptionLevel level;
    bool is_fec_packet;
    size_t length;
  };

  class RetransmissionTimeComparator {
   public:
    bool operator()(const RetransmissionTime& lhs,
                    const RetransmissionTime& rhs) const {
      DCHECK(lhs.scheduled_time.IsInitialized() &&
             rhs.scheduled_time.IsInitialized());
      return lhs.scheduled_time > rhs.scheduled_time;
    }
  };

  typedef std::list<QueuedPacket> QueuedPacketList;
  typedef std::map<QuicFecGroupNumber, QuicFecGroup*> FecGroupMap;
  typedef std::priority_queue<RetransmissionTime,
                              std::vector<RetransmissionTime>,
                              RetransmissionTimeComparator>
      RetransmissionTimeouts;

  
  void SendVersionNegotiationPacket();

  void SetupRetransmissionAlarm(QuicPacketSequenceNumber sequence_number);
  bool IsRetransmission(QuicPacketSequenceNumber sequence_number);

  void SetupAbandonFecTimer(QuicPacketSequenceNumber sequence_number);

  
  void ClearLastFrames();

  
  
  bool DoWrite();

  
  
  
  QuicSequenceNumberLength CalculateSequenceNumberLength(
      QuicPacketSequenceNumber sequence_number);

  
  
  
  
  void DropPacket(QuicPacketSequenceNumber sequence_number);

  
  
  bool WriteQueuedPackets();

  
  void WritePendingRetransmissions();

  
  bool ShouldDiscardPacket(EncryptionLevel level,
                           QuicPacketSequenceNumber sequence_number,
                           HasRetransmittableData retransmittable);

  
  
  void QueueUndecryptablePacket(const QuicEncryptedPacket& packet);

  
  void MaybeProcessUndecryptablePackets();

  
  
  void MaybeProcessRevivedPacket();

  void ProcessAckFrame(const QuicAckFrame& incoming_ack);

  
  void UpdateSentPacketInfo(SentPacketInfo* sent_info);

  
  bool ShouldLastPacketInstigateAck();

  
  
  void MaybeSendInResponseToPacket(bool send_ack_immediately,
                                   bool last_packet_should_instigate_ack);

  
  
  QuicFecGroup* GetFecGroup();

  
  void CloseFecGroupsBefore(QuicPacketSequenceNumber sequence_number);

  QuicConnectionHelperInterface* helper_;  
  QuicPacketWriter* writer_;  
  EncryptionLevel encryption_level_;
  const QuicClock* clock_;
  QuicRandom* random_generator_;

  const QuicGuid guid_;
  
  
  IPEndPoint self_address_;
  IPEndPoint peer_address_;

  bool last_packet_revived_;  
  size_t last_size_;  
  QuicPacketHeader last_header_;
  std::vector<QuicStreamFrame> last_stream_frames_;
  std::vector<QuicAckFrame> last_ack_frames_;
  std::vector<QuicCongestionFeedbackFrame> last_congestion_frames_;
  std::vector<QuicRstStreamFrame> last_rst_frames_;
  std::vector<QuicGoAwayFrame> last_goaway_frames_;
  std::vector<QuicConnectionCloseFrame> last_close_frames_;

  QuicCongestionFeedbackFrame outgoing_congestion_feedback_;

  
  
  QuicPacketSequenceNumber largest_seen_packet_with_ack_;

  
  
  
  
  std::deque<QuicEncryptedPacket*> undecryptable_packets_;

  
  
  bool pending_version_negotiation_packet_;

  
  
  
  QueuedPacketList queued_packets_;

  
  scoped_ptr<PendingWrite> pending_write_;

  
  scoped_ptr<QuicEncryptedPacket> connection_close_packet_;

  
  bool write_blocked_;

  FecGroupMap group_map_;

  QuicReceivedPacketManager received_packet_manager_;
  QuicSentEntropyManager sent_entropy_manager_;

  
  scoped_ptr<QuicAlarm> ack_alarm_;
  
  scoped_ptr<QuicAlarm> retransmission_alarm_;
  
  
  scoped_ptr<QuicAlarm> send_alarm_;
  
  
  scoped_ptr<QuicAlarm> resume_writes_alarm_;
  
  scoped_ptr<QuicAlarm> timeout_alarm_;

  QuicConnectionVisitorInterface* visitor_;
  QuicConnectionDebugVisitorInterface* debug_visitor_;
  QuicPacketCreator packet_creator_;
  QuicPacketGenerator packet_generator_;

  
  QuicTime::Delta idle_network_timeout_;
  
  QuicTime::Delta overall_connection_timeout_;
  
  QuicTime creation_time_;

  
  QuicConnectionStats stats_;

  
  
  QuicTime time_of_last_received_packet_;

  
  QuicTime time_of_last_sent_packet_;

  
  
  
  QuicPacketSequenceNumber sequence_number_of_last_inorder_packet_;

  
  
  
  QuicSentPacketManager sent_packet_manager_;

  
  QuicVersionNegotiationState version_negotiation_state_;

  
  bool is_server_;

  
  
  bool connected_;

  
  
  bool address_migrating_;

  
  
  
  AckNotifierManager ack_notifier_manager_;

  
  
  QuicVersionVector server_supported_versions_;

  DISALLOW_COPY_AND_ASSIGN(QuicConnection);
};

}  

#endif  
