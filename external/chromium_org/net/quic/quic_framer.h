// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_FRAMER_H_
#define NET_QUIC_QUIC_FRAMER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/quic/quic_protocol.h"

namespace net {

namespace test {
class QuicFramerPeer;
}  

class QuicDataReader;
class QuicDataWriter;
class QuicDecrypter;
class QuicEncrypter;
class QuicFramer;

const size_t kQuicFrameTypeSize = 1;
const size_t kQuicErrorCodeSize = 4;
const size_t kQuicErrorDetailsLengthSize = 2;

const size_t kQuicMaxStreamIdSize = 4;
const size_t kQuicMaxStreamOffsetSize = 8;
const size_t kQuicStreamPayloadLengthSize = 2;

const size_t kQuicEntropyHashSize = 1;
const size_t kQuicDeltaTimeLargestObservedSize = 2;
const size_t kNumberOfMissingPacketsSize = 1;

class NET_EXPORT_PRIVATE QuicFramerVisitorInterface {
 public:
  virtual ~QuicFramerVisitorInterface() {}

  
  virtual void OnError(QuicFramer* framer) = 0;

  
  
  
  
  
  virtual bool OnProtocolVersionMismatch(QuicVersion received_version) = 0;

  
  
  virtual void OnPacket() = 0;

  
  
  virtual void OnPublicResetPacket(
      const QuicPublicResetPacket& packet) = 0;

  
  
  virtual void OnVersionNegotiationPacket(
      const QuicVersionNegotiationPacket& packet) = 0;

  
  
  virtual void OnRevivedPacket() = 0;

  
  
  
  virtual bool OnUnauthenticatedHeader(const QuicPacketHeader& header) = 0;

  
  
  virtual bool OnPacketHeader(const QuicPacketHeader& header) = 0;

  
  
  virtual void OnFecProtectedPayload(base::StringPiece payload) = 0;

  
  virtual bool OnStreamFrame(const QuicStreamFrame& frame) = 0;

  
  
  virtual bool OnAckFrame(const QuicAckFrame& frame) = 0;

  
  virtual bool OnCongestionFeedbackFrame(
      const QuicCongestionFeedbackFrame& frame) = 0;

  
  virtual bool OnRstStreamFrame(const QuicRstStreamFrame& frame) = 0;

  
  virtual bool OnConnectionCloseFrame(
      const QuicConnectionCloseFrame& frame) = 0;

  
  virtual bool OnGoAwayFrame(const QuicGoAwayFrame& frame) = 0;

  
  virtual void OnFecData(const QuicFecData& fec) = 0;

  
  virtual void OnPacketComplete() = 0;
};

class NET_EXPORT_PRIVATE QuicFecBuilderInterface {
 public:
  virtual ~QuicFecBuilderInterface() {}

  
  
  virtual void OnBuiltFecProtectedPayload(const QuicPacketHeader& header,
                                          base::StringPiece payload) = 0;
};

class NET_EXPORT_PRIVATE QuicReceivedEntropyHashCalculatorInterface {
 public:
  virtual ~QuicReceivedEntropyHashCalculatorInterface() {}

  
  
  
  
  
  virtual QuicPacketEntropyHash EntropyHash(
      QuicPacketSequenceNumber sequence_number) const = 0;
};

class NET_EXPORT_PRIVATE QuicFramer {
 public:
  
  
  
  
  QuicFramer(const QuicVersionVector& supported_versions,
             QuicTime creation_time,
             bool is_server);

  virtual ~QuicFramer();

  
  bool IsSupportedVersion(const QuicVersion version) const;

  
  static bool HasVersionFlag(const QuicEncryptedPacket& packet);

  
  // Frame was truncated.  last_written in this case is the iterator for the
  
  static QuicPacketSequenceNumber CalculateLargestObserved(
      const SequenceNumberSet& missing_packets,
      SequenceNumberSet::const_iterator last_written);

  
  
  
  
  void set_visitor(QuicFramerVisitorInterface* visitor) {
    visitor_ = visitor;
  }

  
  
  
  void set_fec_builder(QuicFecBuilderInterface* builder) {
    fec_builder_ = builder;
  }

  const QuicVersionVector& supported_versions() const {
    return supported_versions_;
  }

  QuicVersion version() const {
    return quic_version_;
  }

  void set_version(const QuicVersion version);

  
  
  void set_version_for_tests(const QuicVersion version) {
    quic_version_ = version;
  }

  
  
  
  
  void set_received_entropy_calculator(
      QuicReceivedEntropyHashCalculatorInterface* entropy_calculator) {
    entropy_calculator_ = entropy_calculator;
  }

  QuicErrorCode error() const {
    return error_;
  }

  
  
  
  
  
  bool ProcessPacket(const QuicEncryptedPacket& packet);

  
  
  
  
  bool ProcessRevivedPacket(QuicPacketHeader* header,
                            base::StringPiece payload);

  
  static size_t GetMinStreamFrameSize(QuicVersion version,
                                      QuicStreamId stream_id,
                                      QuicStreamOffset offset,
                                      bool last_frame_in_packet);
  
  static size_t GetMinAckFrameSize(
      QuicVersion version,
      QuicSequenceNumberLength sequence_number_length,
      QuicSequenceNumberLength largest_observed_length);
  
  static size_t GetMinRstStreamFrameSize();
  
  
  static size_t GetMinConnectionCloseFrameSize();
  
  static size_t GetMinGoAwayFrameSize();
  
  
  static size_t GetMaxUnackedPackets(QuicPacketHeader header);
  
  static size_t GetStreamIdSize(QuicStreamId stream_id);
  
  static size_t GetStreamOffsetSize(QuicStreamOffset offset);
  
  static size_t GetVersionNegotiationPacketSize(size_t number_versions);


  static bool CanTruncate(
      QuicVersion version, const QuicFrame& frame, size_t free_bytes);

  
  
  
  size_t GetSerializedFrameLength(
      const QuicFrame& frame,
      size_t free_bytes,
      bool first_frame,
      bool last_frame,
      QuicSequenceNumberLength sequence_number_length);

  
  
  static base::StringPiece GetAssociatedDataFromEncryptedPacket(
      const QuicEncryptedPacket& encrypted,
      QuicGuidLength guid_length,
      bool includes_version,
      QuicSequenceNumberLength sequence_number_length);

  
  
  
  
  SerializedPacket BuildUnsizedDataPacket(const QuicPacketHeader& header,
                                          const QuicFrames& frames);

  
  
  
  SerializedPacket BuildDataPacket(const QuicPacketHeader& header,
                                   const QuicFrames& frames,
                                   size_t packet_size);

  
  
  
  SerializedPacket BuildFecPacket(const QuicPacketHeader& header,
                                  const QuicFecData& fec);

  
  static QuicEncryptedPacket* BuildPublicResetPacket(
      const QuicPublicResetPacket& packet);

  QuicEncryptedPacket* BuildVersionNegotiationPacket(
      const QuicPacketPublicHeader& header,
      const QuicVersionVector& supported_versions);

  
  
  
  
  
  void SetDecrypter(QuicDecrypter* decrypter);

  
  
  
  
  
  void SetAlternativeDecrypter(QuicDecrypter* decrypter,
                               bool latch_once_used);

  const QuicDecrypter* decrypter() const;
  const QuicDecrypter* alternative_decrypter() const;

  
  
  void SetEncrypter(EncryptionLevel level, QuicEncrypter* encrypter);
  const QuicEncrypter* encrypter(EncryptionLevel level) const;

  
  
  void SwapCryptersForTest(QuicFramer* other);

  
  QuicEncryptedPacket* EncryptPacket(EncryptionLevel level,
                                     QuicPacketSequenceNumber sequence_number,
                                     const QuicPacket& packet);

  
  
  size_t GetMaxPlaintextSize(size_t ciphertext_size);

  const std::string& detailed_error() { return detailed_error_; }

  
  
  static bool ReadGuidFromPacket(const QuicEncryptedPacket& packet,
                                 QuicGuid* guid);

  static QuicSequenceNumberLength ReadSequenceNumberLength(uint8 flags);

  
  static QuicSequenceNumberLength GetMinSequenceNumberLength(
      QuicPacketSequenceNumber sequence_number);

 private:
  friend class test::QuicFramerPeer;

  typedef std::map<QuicPacketSequenceNumber, uint8> NackRangeMap;

  struct AckFrameInfo {
    AckFrameInfo();
    ~AckFrameInfo();

    
    QuicPacketSequenceNumber max_delta;
    
    NackRangeMap nack_ranges;
  };

  QuicPacketEntropyHash GetPacketEntropyHash(
      const QuicPacketHeader& header) const;

  bool ProcessDataPacket(const QuicPacketPublicHeader& public_header,
                         const QuicEncryptedPacket& packet);

  bool ProcessPublicResetPacket(const QuicPacketPublicHeader& public_header);

  bool ProcessVersionNegotiationPacket(QuicPacketPublicHeader* public_header);

  bool ProcessPublicHeader(QuicPacketPublicHeader* header);

  bool ProcessPacketHeader(QuicPacketHeader* header,
                           const QuicEncryptedPacket& packet);

  bool ProcessPacketSequenceNumber(
      QuicSequenceNumberLength sequence_number_length,
      QuicPacketSequenceNumber* sequence_number);
  bool ProcessFrameData(const QuicPacketHeader& header);
  bool ProcessStreamFrame(uint8 frame_type, QuicStreamFrame* frame);
  bool ProcessAckFrame(const QuicPacketHeader& header,
                       uint8 frame_type,
                       QuicAckFrame* frame);
  bool ProcessReceivedInfo(uint8 frame_type, ReceivedPacketInfo* received_info);
  bool ProcessSentInfo(const QuicPacketHeader& public_header,
                       SentPacketInfo* sent_info);
  bool ProcessQuicCongestionFeedbackFrame(
      QuicCongestionFeedbackFrame* congestion_feedback);
  bool ProcessRstStreamFrame(QuicRstStreamFrame* frame);
  bool ProcessConnectionCloseFrame(QuicConnectionCloseFrame* frame);
  bool ProcessGoAwayFrame(QuicGoAwayFrame* frame);

  bool DecryptPayload(const QuicPacketHeader& header,
                      const QuicEncryptedPacket& packet);

  
  
  QuicPacketSequenceNumber CalculatePacketSequenceNumberFromWire(
      QuicSequenceNumberLength sequence_number_length,
      QuicPacketSequenceNumber packet_sequence_number) const;

  
  size_t GetAckFrameSize(const QuicAckFrame& ack,
                         QuicSequenceNumberLength sequence_number_length);

  
  size_t ComputeFrameLength(const QuicFrame& frame,
                            bool last_frame_in_packet,
                            QuicSequenceNumberLength sequence_number_length);

  static bool AppendPacketSequenceNumber(
      QuicSequenceNumberLength sequence_number_length,
      QuicPacketSequenceNumber packet_sequence_number,
      QuicDataWriter* writer);

  static uint8 GetSequenceNumberFlags(
      QuicSequenceNumberLength sequence_number_length);

  static AckFrameInfo GetAckFrameInfo(const QuicAckFrame& frame);

  bool AppendPacketHeader(const QuicPacketHeader& header,
                          QuicDataWriter* writer);
  bool AppendTypeByte(const QuicFrame& frame,
                      bool last_frame_in_packet,
                      QuicDataWriter* writer);
  bool AppendStreamFramePayload(const QuicStreamFrame& frame,
                                bool last_frame_in_packet,
                                QuicDataWriter* builder);
  bool AppendAckFramePayloadAndTypeByte(const QuicPacketHeader& header,
                                        const QuicAckFrame& frame,
                                        QuicDataWriter* builder);
  bool AppendQuicCongestionFeedbackFramePayload(
      const QuicCongestionFeedbackFrame& frame,
      QuicDataWriter* builder);
  bool AppendRstStreamFramePayload(const QuicRstStreamFrame& frame,
                                   QuicDataWriter* builder);
  bool AppendConnectionCloseFramePayload(
      const QuicConnectionCloseFrame& frame,
      QuicDataWriter* builder);
  bool AppendGoAwayFramePayload(const QuicGoAwayFrame& frame,
                                QuicDataWriter* writer);
  bool RaiseError(QuicErrorCode error);

  void set_error(QuicErrorCode error) {
    error_ = error;
  }

  void set_detailed_error(const char* error) {
    detailed_error_ = error;
  }

  std::string detailed_error_;
  scoped_ptr<QuicDataReader> reader_;
  QuicFramerVisitorInterface* visitor_;
  QuicFecBuilderInterface* fec_builder_;
  QuicReceivedEntropyHashCalculatorInterface* entropy_calculator_;
  QuicErrorCode error_;
  
  QuicPacketSequenceNumber last_sequence_number_;
  
  QuicGuid last_serialized_guid_;
  
  scoped_ptr<QuicData> decrypted_;
  
  QuicVersion quic_version_;
  
  
  
  
  QuicVersionVector supported_versions_;
  
  scoped_ptr<QuicDecrypter> decrypter_;
  
  scoped_ptr<QuicDecrypter> alternative_decrypter_;
  
  
  
  bool alternative_decrypter_latch_;
  
  scoped_ptr<QuicEncrypter> encrypter_[NUM_ENCRYPTION_LEVELS];
  
  
  bool is_server_;
  // The time this frames was created.  Time written to the wire will be
  // written as a delta from this value.
  QuicTime creation_time_;

  DISALLOW_COPY_AND_ASSIGN(QuicFramer);
};

}  

#endif  
