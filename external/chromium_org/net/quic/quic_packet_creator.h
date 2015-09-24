// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_PACKET_CREATOR_H_
#define NET_QUIC_QUIC_PACKET_CREATOR_H_

#include <utility>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "net/quic/quic_fec_group.h"
#include "net/quic/quic_framer.h"
#include "net/quic/quic_protocol.h"

namespace net {
namespace test {
class QuicPacketCreatorPeer;
}

class QuicAckNotifier;
class QuicRandom;
class QuicRandomBoolSource;

class NET_EXPORT_PRIVATE QuicPacketCreator : public QuicFecBuilderInterface {
 public:
  
  struct Options {
    Options()
        : max_packet_length(kDefaultMaxPacketSize),
          max_packets_per_fec_group(0),
          send_guid_length(PACKET_8BYTE_GUID),
          send_sequence_number_length(PACKET_1BYTE_SEQUENCE_NUMBER) {}

    size_t max_packet_length;
    
    size_t max_packets_per_fec_group;
    
    QuicGuidLength send_guid_length;
    QuicSequenceNumberLength send_sequence_number_length;
  };

  
  QuicPacketCreator(QuicGuid guid,
                    QuicFramer* framer,
                    QuicRandom* random_generator,
                    bool is_server);

  virtual ~QuicPacketCreator();

  
  virtual void OnBuiltFecProtectedPayload(const QuicPacketHeader& header,
                                          base::StringPiece payload) OVERRIDE;

  
  
  bool ShouldSendFec(bool force_close) const;

  
  void StopSendingVersion();

  
  
  void UpdateSequenceNumberLength(
      QuicPacketSequenceNumber least_packet_awaited_by_peer,
      QuicByteCount bytes_per_second);

  
  static size_t StreamFramePacketOverhead(
      QuicVersion version,
      QuicGuidLength guid_length,
      bool include_version,
      QuicSequenceNumberLength sequence_number_length,
      InFecGroup is_in_fec_group);

  bool HasRoomForStreamFrame(QuicStreamId id, QuicStreamOffset offset) const;

  
  
  
  
  size_t CreateStreamFrame(QuicStreamId id,
                           const IOVector& data,
                           QuicStreamOffset offset,
                           bool fin,
                           QuicFrame* frame);

  
  
  
  
  size_t CreateStreamFrameWithNotifier(QuicStreamId id,
                                       const IOVector& data,
                                       QuicStreamOffset offset,
                                       bool fin,
                                       QuicAckNotifier* notifier,
                                       QuicFrame* frame);

  
  
  
  
  SerializedPacket SerializeAllFrames(const QuicFrames& frames);

  
  
  SerializedPacket ReserializeAllFrames(
      const QuicFrames& frames, QuicSequenceNumberLength original_length);

  
  bool HasPendingFrames();

  
  
  
  
  size_t BytesFree() const;

  
  
  
  
  size_t PacketSize() const;

  
  
  
  
  
  bool AddSavedFrame(const QuicFrame& frame);

  
  
  
  
  
  
  
  SerializedPacket SerializePacket();

  
  
  
  SerializedPacket SerializeFec();

  
  
  
  SerializedPacket SerializeConnectionClose(
      QuicConnectionCloseFrame* close_frame);

  
  
  
  
  QuicEncryptedPacket* SerializeVersionNegotiationPacket(
      const QuicVersionVector& supported_versions);

  
  
  QuicPacketSequenceNumber sequence_number() const {
    return sequence_number_;
  }

  void set_sequence_number(QuicPacketSequenceNumber s) {
    sequence_number_ = s;
  }

  Options* options() {
    return &options_;
  }

 private:
  friend class test::QuicPacketCreatorPeer;

  static bool ShouldRetransmit(const QuicFrame& frame);

  
  
  void MaybeStartFEC();

  void FillPacketHeader(QuicFecGroupNumber fec_group,
                        bool fec_flag,
                        bool fec_entropy_flag,
                        QuicPacketHeader* header);

  
  
  bool AddFrame(const QuicFrame& frame, bool save_retransmittable_frames);

  
  
  
  void MaybeAddPadding();

  Options options_;
  QuicGuid guid_;
  QuicFramer* framer_;
  scoped_ptr<QuicRandomBoolSource> random_bool_source_;
  QuicPacketSequenceNumber sequence_number_;
  QuicFecGroupNumber fec_group_number_;
  scoped_ptr<QuicFecGroup> fec_group_;
  
  bool is_server_;
  
  
  bool send_version_in_packet_;
  
  
  
  mutable QuicSequenceNumberLength sequence_number_length_;
  
  
  mutable size_t packet_size_;
  QuicFrames queued_frames_;
  scoped_ptr<RetransmittableFrames> queued_retransmittable_frames_;

  DISALLOW_COPY_AND_ASSIGN(QuicPacketCreator);
};

}  

#endif  
