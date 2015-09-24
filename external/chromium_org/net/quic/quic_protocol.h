// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_PROTOCOL_H_
#define NET_QUIC_QUIC_PROTOCOL_H_

#include <stddef.h>
#include <limits>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/logging.h"
#include "base/strings/string_piece.h"
#include "net/base/int128.h"
#include "net/base/net_export.h"
#include "net/quic/iovector.h"
#include "net/quic/quic_bandwidth.h"
#include "net/quic/quic_time.h"

namespace net {

using ::operator<<;

class QuicAckNotifier;
class QuicPacket;
struct QuicPacketHeader;

typedef uint64 QuicGuid;
typedef uint32 QuicStreamId;
typedef uint64 QuicStreamOffset;
typedef uint64 QuicPacketSequenceNumber;
typedef QuicPacketSequenceNumber QuicFecGroupNumber;
typedef uint64 QuicPublicResetNonceProof;
typedef uint8 QuicPacketEntropyHash;
typedef uint32 QuicHeaderId;
typedef uint32 QuicTag;
typedef std::vector<QuicTag> QuicTagVector;
typedef uint32 QuicPriority;

const QuicByteCount kDefaultMaxPacketSize = 1200;
const QuicByteCount kMaxPacketSize = 1452;

const size_t kDefaultInitialWindow = 10;
const size_t kMaxInitialWindow = 10;

const size_t kMaxTcpCongestionWindow = 200;

const size_t kMaxInitialRoundTripTimeUs = 15 * kNumMicrosPerSecond;

const size_t kDefaultMaxStreamsPerConnection = 100;

const size_t kPublicFlagsSize = 1;
const size_t kQuicVersionSize = 4;
const size_t kPrivateFlagsSize = 1;
const size_t kFecGroupSize = 1;
const size_t kPublicResetNonceSize = 8;

const bool kIncludeVersion = true;

const size_t kStartOfHashData = 0;

const QuicStreamId kMaxStreamIdDelta = 100;
const QuicHeaderId kMaxHeaderIdDelta = 100;

const QuicStreamId kCryptoStreamId = 1;

const int64 kDefaultInitialTimeoutSecs = 120;  
const int64 kDefaultTimeoutSecs = 60 * 10;  
const int64 kDefaultMaxTimeForCryptoHandshakeSecs = 5;  

const int kUFloat16ExponentBits = 5;
const int kUFloat16MaxExponent = (1 << kUFloat16ExponentBits) - 2;  
const int kUFloat16MantissaBits = 16 - kUFloat16ExponentBits;  
const int kUFloat16MantissaEffectiveBits = kUFloat16MantissaBits + 1;  
const uint64 kUFloat16MaxValue =  
    ((GG_UINT64_C(1) << kUFloat16MantissaEffectiveBits) - 1) <<
    kUFloat16MaxExponent;

enum TransmissionType {
  NOT_RETRANSMISSION,
  NACK_RETRANSMISSION,
  RTO_RETRANSMISSION,
};

enum RetransmissionType {
  INITIAL_ENCRYPTION_ONLY,
  ALL_PACKETS
};

enum HasRetransmittableData {
  NO_RETRANSMITTABLE_DATA,
  HAS_RETRANSMITTABLE_DATA,
};

enum IsHandshake {
  NOT_HANDSHAKE,
  IS_HANDSHAKE
};

enum QuicFrameType {
  PADDING_FRAME = 0,
  RST_STREAM_FRAME,
  CONNECTION_CLOSE_FRAME,
  GOAWAY_FRAME,
  STREAM_FRAME,
  ACK_FRAME,
  CONGESTION_FEEDBACK_FRAME,
  NUM_FRAME_TYPES
};

enum QuicGuidLength {
  PACKET_0BYTE_GUID = 0,
  PACKET_1BYTE_GUID = 1,
  PACKET_4BYTE_GUID = 4,
  PACKET_8BYTE_GUID = 8
};

enum InFecGroup {
  NOT_IN_FEC_GROUP,
  IN_FEC_GROUP,
};

enum QuicSequenceNumberLength {
  PACKET_1BYTE_SEQUENCE_NUMBER = 1,
  PACKET_2BYTE_SEQUENCE_NUMBER = 2,
  PACKET_4BYTE_SEQUENCE_NUMBER = 4,
  PACKET_6BYTE_SEQUENCE_NUMBER = 6
};

enum QuicSequenceNumberLengthFlags {
  PACKET_FLAGS_1BYTE_SEQUENCE = 0,  
  PACKET_FLAGS_2BYTE_SEQUENCE = 1,  
  PACKET_FLAGS_4BYTE_SEQUENCE = 1 << 1,  
  PACKET_FLAGS_6BYTE_SEQUENCE = 1 << 1 | 1,  
};

enum QuicPacketPublicFlags {
  PACKET_PUBLIC_FLAGS_NONE = 0,

  
  PACKET_PUBLIC_FLAGS_VERSION = 1 << 0,

  
  PACKET_PUBLIC_FLAGS_RST = 1 << 1,

  
  
  
  
  
  PACKET_PUBLIC_FLAGS_0BYTE_GUID = 0,
  PACKET_PUBLIC_FLAGS_1BYTE_GUID = 1 << 2,
  PACKET_PUBLIC_FLAGS_4BYTE_GUID = 1 << 3,
  PACKET_PUBLIC_FLAGS_8BYTE_GUID = 1 << 3 | 1 << 2,

  
  
  
  
  
  PACKET_PUBLIC_FLAGS_1BYTE_SEQUENCE = PACKET_FLAGS_1BYTE_SEQUENCE << 4,
  PACKET_PUBLIC_FLAGS_2BYTE_SEQUENCE = PACKET_FLAGS_2BYTE_SEQUENCE << 4,
  PACKET_PUBLIC_FLAGS_4BYTE_SEQUENCE = PACKET_FLAGS_4BYTE_SEQUENCE << 4,
  PACKET_PUBLIC_FLAGS_6BYTE_SEQUENCE = PACKET_FLAGS_6BYTE_SEQUENCE << 4,

  
  PACKET_PUBLIC_FLAGS_MAX = (1 << 6) - 1
};

enum QuicPacketPrivateFlags {
  PACKET_PRIVATE_FLAGS_NONE = 0,

  
  PACKET_PRIVATE_FLAGS_ENTROPY = 1 << 0,

  
  PACKET_PRIVATE_FLAGS_FEC_GROUP = 1 << 1,

  
  PACKET_PRIVATE_FLAGS_FEC = 1 << 2,

  
  PACKET_PRIVATE_FLAGS_MAX = (1 << 3) - 1
};

enum QuicVersion {
  
  QUIC_VERSION_UNSUPPORTED = 0,

  QUIC_VERSION_12 = 12,  
};

static const QuicVersion kSupportedQuicVersions[] = {QUIC_VERSION_12};

typedef std::vector<QuicVersion> QuicVersionVector;

NET_EXPORT_PRIVATE QuicVersionVector QuicSupportedVersions();

// QuicTag is written to and read from the wire, but we prefer to use
NET_EXPORT_PRIVATE QuicTag QuicVersionToQuicTag(const QuicVersion version);

NET_EXPORT_PRIVATE QuicVersion QuicTagToQuicVersion(const QuicTag version_tag);

NET_EXPORT_PRIVATE std::string QuicVersionToString(const QuicVersion version);

NET_EXPORT_PRIVATE std::string QuicVersionVectorToString(
    const QuicVersionVector& versions);

// Version and Crypto tags are written to the wire with a big-endian
// the client hello tag (CHLO) will be written as the

NET_EXPORT_PRIVATE QuicTag MakeQuicTag(char a, char b, char c, char d);

NET_EXPORT_PRIVATE size_t GetPacketHeaderSize(QuicPacketHeader header);

NET_EXPORT_PRIVATE size_t GetPacketHeaderSize(
    QuicGuidLength guid_length,
    bool include_version,
    QuicSequenceNumberLength sequence_number_length,
    InFecGroup is_in_fec_group);

NET_EXPORT_PRIVATE size_t GetPublicResetPacketSize();

NET_EXPORT_PRIVATE size_t GetStartOfFecProtectedData(
    QuicGuidLength guid_length,
    bool include_version,
    QuicSequenceNumberLength sequence_number_length);
NET_EXPORT_PRIVATE size_t GetStartOfEncryptedData(
    QuicGuidLength guid_length,
    bool include_version,
    QuicSequenceNumberLength sequence_number_length);

enum QuicRstStreamErrorCode {
  QUIC_STREAM_NO_ERROR = 0,

  
  QUIC_ERROR_PROCESSING_STREAM,
  
  QUIC_MULTIPLE_TERMINATION_OFFSETS,
  
  QUIC_BAD_APPLICATION_PAYLOAD,
  
  
  QUIC_STREAM_CONNECTION_ERROR,
  
  QUIC_STREAM_PEER_GOING_AWAY,
  
  QUIC_STREAM_CANCELLED,

  
  QUIC_STREAM_LAST_ERROR,
};

enum QuicErrorCode {
  QUIC_NO_ERROR = 0,

  
  QUIC_INTERNAL_ERROR = 1,
  
  QUIC_STREAM_DATA_AFTER_TERMINATION = 2,
  
  QUIC_INVALID_PACKET_HEADER = 3,
  
  QUIC_INVALID_FRAME_DATA = 4,
  
  QUIC_MISSING_PAYLOAD = 48,
  
  QUIC_INVALID_FEC_DATA = 5,
  
  QUIC_INVALID_STREAM_DATA = 46,
  
  QUIC_INVALID_RST_STREAM_DATA = 6,
  
  QUIC_INVALID_CONNECTION_CLOSE_DATA = 7,
  
  QUIC_INVALID_GOAWAY_DATA = 8,
  
  QUIC_INVALID_ACK_DATA = 9,
  
  QUIC_INVALID_CONGESTION_FEEDBACK_DATA = 47,
  
  QUIC_INVALID_VERSION_NEGOTIATION_PACKET = 10,
  
  QUIC_INVALID_PUBLIC_RST_PACKET = 11,
  
  QUIC_DECRYPTION_FAILURE = 12,
  
  QUIC_ENCRYPTION_FAILURE = 13,
  
  QUIC_PACKET_TOO_LARGE = 14,
  
  QUIC_PACKET_FOR_NONEXISTENT_STREAM = 15,
  
  QUIC_PEER_GOING_AWAY = 16,
  
  QUIC_INVALID_STREAM_ID = 17,
  
  QUIC_INVALID_PRIORITY = 49,
  
  QUIC_TOO_MANY_OPEN_STREAMS = 18,
  
  QUIC_PUBLIC_RESET = 19,
  
  QUIC_INVALID_VERSION = 20,
  
  QUIC_STREAM_RST_BEFORE_HEADERS_DECOMPRESSED = 21,
  
  QUIC_INVALID_HEADER_ID = 22,
  
  QUIC_INVALID_NEGOTIATED_VALUE = 23,
  
  QUIC_DECOMPRESSION_FAILURE = 24,
  
  QUIC_CONNECTION_TIMED_OUT = 25,
  
  QUIC_ERROR_MIGRATING_ADDRESS = 26,
  
  QUIC_PACKET_WRITE_ERROR = 27,
  
  QUIC_PACKET_READ_ERROR = 51,
  
  QUIC_INVALID_STREAM_FRAME = 50,


  

  
  QUIC_HANDSHAKE_FAILED = 28,
  
  QUIC_CRYPTO_TAGS_OUT_OF_ORDER = 29,
  
  QUIC_CRYPTO_TOO_MANY_ENTRIES = 30,
  
  QUIC_CRYPTO_INVALID_VALUE_LENGTH = 31,
  
  QUIC_CRYPTO_MESSAGE_AFTER_HANDSHAKE_COMPLETE = 32,
  
  QUIC_INVALID_CRYPTO_MESSAGE_TYPE = 33,
  
  QUIC_INVALID_CRYPTO_MESSAGE_PARAMETER = 34,
  
  QUIC_INVALID_CHANNEL_ID_SIGNATURE = 52,
  
  QUIC_CRYPTO_MESSAGE_PARAMETER_NOT_FOUND = 35,
  
  
  QUIC_CRYPTO_MESSAGE_PARAMETER_NO_OVERLAP = 36,
  
  
  QUIC_CRYPTO_MESSAGE_INDEX_NOT_FOUND = 37,
  
  QUIC_CRYPTO_INTERNAL_ERROR = 38,
  
  QUIC_CRYPTO_VERSION_NOT_SUPPORTED = 39,
  
  
  QUIC_CRYPTO_NO_SUPPORT = 40,
  
  QUIC_CRYPTO_TOO_MANY_REJECTS = 41,
  
  QUIC_PROOF_INVALID = 42,
  
  QUIC_CRYPTO_DUPLICATE_TAG = 43,
  
  
  QUIC_CRYPTO_ENCRYPTION_LEVEL_INCORRECT = 44,
  
  QUIC_CRYPTO_SERVER_CONFIG_EXPIRED = 45,
  
  QUIC_CRYPTO_SYMMETRIC_KEY_SETUP_FAILED = 53,
  
  
  QUIC_CRYPTO_MESSAGE_WHILE_VALIDATING_CLIENT_HELLO = 54,
  
  
  QUIC_VERSION_NEGOTIATION_MISMATCH = 55,

  
  QUIC_LAST_ERROR = 56,
};

struct NET_EXPORT_PRIVATE QuicPacketPublicHeader {
  QuicPacketPublicHeader();
  explicit QuicPacketPublicHeader(const QuicPacketPublicHeader& other);
  ~QuicPacketPublicHeader();

  
  QuicGuid guid;
  QuicGuidLength guid_length;
  bool reset_flag;
  bool version_flag;
  QuicSequenceNumberLength sequence_number_length;
  QuicVersionVector versions;
};

struct NET_EXPORT_PRIVATE QuicPacketHeader {
  QuicPacketHeader();
  explicit QuicPacketHeader(const QuicPacketPublicHeader& header);

  NET_EXPORT_PRIVATE friend std::ostream& operator<<(
      std::ostream& os, const QuicPacketHeader& s);

  QuicPacketPublicHeader public_header;
  bool fec_flag;
  bool entropy_flag;
  QuicPacketEntropyHash entropy_hash;
  QuicPacketSequenceNumber packet_sequence_number;
  InFecGroup is_in_fec_group;
  QuicFecGroupNumber fec_group;
};

struct NET_EXPORT_PRIVATE QuicPublicResetPacket {
  QuicPublicResetPacket() {}
  explicit QuicPublicResetPacket(const QuicPacketPublicHeader& header)
      : public_header(header) {}
  QuicPacketPublicHeader public_header;
  QuicPacketSequenceNumber rejected_sequence_number;
  QuicPublicResetNonceProof nonce_proof;
};

enum QuicVersionNegotiationState {
  START_NEGOTIATION = 0,
  
  
  
  
  
  NEGOTIATION_IN_PROGRESS,
  
  
  
  NEGOTIATED_VERSION
};

typedef QuicPacketPublicHeader QuicVersionNegotiationPacket;

struct NET_EXPORT_PRIVATE QuicPaddingFrame {
};

struct NET_EXPORT_PRIVATE QuicStreamFrame {
  QuicStreamFrame();
  QuicStreamFrame(const QuicStreamFrame& frame);
  QuicStreamFrame(QuicStreamId stream_id,
                  bool fin,
                  QuicStreamOffset offset,
                  IOVector data);

  
  
  std::string* GetDataAsString() const;

  QuicStreamId stream_id;
  bool fin;
  QuicStreamOffset offset;  
  IOVector data;

  
  
  QuicAckNotifier* notifier;
};

typedef std::set<QuicPacketSequenceNumber> SequenceNumberSet;
typedef std::map<QuicPacketSequenceNumber, QuicTime> TimeMap;

struct NET_EXPORT_PRIVATE ReceivedPacketInfo {
  ReceivedPacketInfo();
  ~ReceivedPacketInfo();
  NET_EXPORT_PRIVATE friend std::ostream& operator<<(
      std::ostream& os, const ReceivedPacketInfo& s);

  
  
  QuicPacketEntropyHash entropy_hash;

  
  
  
  
  
  
  
  
  QuicPacketSequenceNumber largest_observed;

  
  
  QuicTime::Delta delta_time_largest_observed;

  
  
  
  SequenceNumberSet missing_packets;

  
  bool is_truncated;
};

bool NET_EXPORT_PRIVATE IsAwaitingPacket(
    const ReceivedPacketInfo& received_info,
    QuicPacketSequenceNumber sequence_number);

void NET_EXPORT_PRIVATE InsertMissingPacketsBetween(
    ReceivedPacketInfo* received_info,
    QuicPacketSequenceNumber lower,
    QuicPacketSequenceNumber higher);

struct NET_EXPORT_PRIVATE SentPacketInfo {
  SentPacketInfo();
  ~SentPacketInfo();
  NET_EXPORT_PRIVATE friend std::ostream& operator<<(
      std::ostream& os, const SentPacketInfo& s);

  
  
  QuicPacketEntropyHash entropy_hash;
  
  QuicPacketSequenceNumber least_unacked;
};

struct NET_EXPORT_PRIVATE QuicAckFrame {
  QuicAckFrame() {}
  
  
  QuicAckFrame(QuicPacketSequenceNumber largest_observed,
               QuicTime largest_observed_receive_time,
               QuicPacketSequenceNumber least_unacked);

  NET_EXPORT_PRIVATE friend std::ostream& operator<<(
      std::ostream& os, const QuicAckFrame& s);

  SentPacketInfo sent_info;
  ReceivedPacketInfo received_info;
};

enum CongestionFeedbackType {
  kTCP,  
  kInterArrival,  
  kFixRate,  
};

struct NET_EXPORT_PRIVATE CongestionFeedbackMessageTCP {
  uint16 accumulated_number_of_lost_packets;
  QuicByteCount receive_window;
};

struct NET_EXPORT_PRIVATE CongestionFeedbackMessageInterArrival {
  CongestionFeedbackMessageInterArrival();
  ~CongestionFeedbackMessageInterArrival();
  uint16 accumulated_number_of_lost_packets;
  
  
  TimeMap received_packet_times;
};

struct NET_EXPORT_PRIVATE CongestionFeedbackMessageFixRate {
  CongestionFeedbackMessageFixRate();
  QuicBandwidth bitrate;
};

struct NET_EXPORT_PRIVATE QuicCongestionFeedbackFrame {
  QuicCongestionFeedbackFrame();
  ~QuicCongestionFeedbackFrame();

  NET_EXPORT_PRIVATE friend std::ostream& operator<<(
      std::ostream& os, const QuicCongestionFeedbackFrame& c);

  CongestionFeedbackType type;
  
  
  CongestionFeedbackMessageTCP tcp;
  CongestionFeedbackMessageInterArrival inter_arrival;
  CongestionFeedbackMessageFixRate fix_rate;
};

struct NET_EXPORT_PRIVATE QuicRstStreamFrame {
  QuicRstStreamFrame() {}
  QuicRstStreamFrame(QuicStreamId stream_id, QuicRstStreamErrorCode error_code)
      : stream_id(stream_id), error_code(error_code) {
    DCHECK_LE(error_code, std::numeric_limits<uint8>::max());
  }

  QuicStreamId stream_id;
  QuicRstStreamErrorCode error_code;
  std::string error_details;
};

struct NET_EXPORT_PRIVATE QuicConnectionCloseFrame {
  QuicErrorCode error_code;
  std::string error_details;
};

struct NET_EXPORT_PRIVATE QuicGoAwayFrame {
  QuicGoAwayFrame() {}
  QuicGoAwayFrame(QuicErrorCode error_code,
                  QuicStreamId last_good_stream_id,
                  const std::string& reason);

  QuicErrorCode error_code;
  QuicStreamId last_good_stream_id;
  std::string reason_phrase;
};

enum EncryptionLevel {
  ENCRYPTION_NONE = 0,
  ENCRYPTION_INITIAL = 1,
  ENCRYPTION_FORWARD_SECURE = 2,

  NUM_ENCRYPTION_LEVELS,
};

struct NET_EXPORT_PRIVATE QuicFrame {
  QuicFrame() {}
  explicit QuicFrame(QuicPaddingFrame* padding_frame)
      : type(PADDING_FRAME),
        padding_frame(padding_frame) {
  }
  explicit QuicFrame(QuicStreamFrame* stream_frame)
      : type(STREAM_FRAME),
        stream_frame(stream_frame) {
  }
  explicit QuicFrame(QuicAckFrame* frame)
      : type(ACK_FRAME),
        ack_frame(frame) {
  }
  explicit QuicFrame(QuicCongestionFeedbackFrame* frame)
      : type(CONGESTION_FEEDBACK_FRAME),
        congestion_feedback_frame(frame) {
  }
  explicit QuicFrame(QuicRstStreamFrame* frame)
      : type(RST_STREAM_FRAME),
        rst_stream_frame(frame) {
  }
  explicit QuicFrame(QuicConnectionCloseFrame* frame)
      : type(CONNECTION_CLOSE_FRAME),
        connection_close_frame(frame) {
  }
  explicit QuicFrame(QuicGoAwayFrame* frame)
      : type(GOAWAY_FRAME),
        goaway_frame(frame) {
  }

  QuicFrameType type;
  union {
    QuicPaddingFrame* padding_frame;
    QuicStreamFrame* stream_frame;
    QuicAckFrame* ack_frame;
    QuicCongestionFeedbackFrame* congestion_feedback_frame;
    QuicRstStreamFrame* rst_stream_frame;
    QuicConnectionCloseFrame* connection_close_frame;
    QuicGoAwayFrame* goaway_frame;
  };
};

typedef std::vector<QuicFrame> QuicFrames;

struct NET_EXPORT_PRIVATE QuicFecData {
  QuicFecData();

  
  
  
  QuicFecGroupNumber fec_group;
  base::StringPiece redundancy;
};

class NET_EXPORT_PRIVATE QuicData {
 public:
  QuicData(const char* buffer, size_t length)
      : buffer_(buffer),
        length_(length),
        owns_buffer_(false) {}

  QuicData(char* buffer, size_t length, bool owns_buffer)
      : buffer_(buffer),
        length_(length),
        owns_buffer_(owns_buffer) {}

  virtual ~QuicData();

  base::StringPiece AsStringPiece() const {
    return base::StringPiece(data(), length());
  }

  const char* data() const { return buffer_; }
  size_t length() const { return length_; }

 private:
  const char* buffer_;
  size_t length_;
  bool owns_buffer_;

  DISALLOW_COPY_AND_ASSIGN(QuicData);
};

class NET_EXPORT_PRIVATE QuicPacket : public QuicData {
 public:
  static QuicPacket* NewDataPacket(
      char* buffer,
      size_t length,
      bool owns_buffer,
      QuicGuidLength guid_length,
      bool includes_version,
      QuicSequenceNumberLength sequence_number_length) {
    return new QuicPacket(buffer, length, owns_buffer, guid_length,
                          includes_version, sequence_number_length, false);
  }

  static QuicPacket* NewFecPacket(
      char* buffer,
      size_t length,
      bool owns_buffer,
      QuicGuidLength guid_length,
      bool includes_version,
      QuicSequenceNumberLength sequence_number_length) {
    return new QuicPacket(buffer, length, owns_buffer, guid_length,
                          includes_version, sequence_number_length, true);
  }

  base::StringPiece FecProtectedData() const;
  base::StringPiece AssociatedData() const;
  base::StringPiece BeforePlaintext() const;
  base::StringPiece Plaintext() const;

  bool is_fec_packet() const { return is_fec_packet_; }

  char* mutable_data() { return buffer_; }

 private:
  QuicPacket(char* buffer,
             size_t length,
             bool owns_buffer,
             QuicGuidLength guid_length,
             bool includes_version,
             QuicSequenceNumberLength sequence_number_length,
             bool is_fec_packet)
      : QuicData(buffer, length, owns_buffer),
        buffer_(buffer),
        is_fec_packet_(is_fec_packet),
        guid_length_(guid_length),
        includes_version_(includes_version),
        sequence_number_length_(sequence_number_length) {}

  char* buffer_;
  const bool is_fec_packet_;
  const QuicGuidLength guid_length_;
  const bool includes_version_;
  const QuicSequenceNumberLength sequence_number_length_;

  DISALLOW_COPY_AND_ASSIGN(QuicPacket);
};

class NET_EXPORT_PRIVATE QuicEncryptedPacket : public QuicData {
 public:
  QuicEncryptedPacket(const char* buffer, size_t length)
      : QuicData(buffer, length) {}

  QuicEncryptedPacket(char* buffer, size_t length, bool owns_buffer)
      : QuicData(buffer, length, owns_buffer) {}

  
  QuicEncryptedPacket* Clone() const;

  
  
  
  
  NET_EXPORT_PRIVATE friend std::ostream& operator<<(
      std::ostream& os, const QuicEncryptedPacket& s);

 private:
  DISALLOW_COPY_AND_ASSIGN(QuicEncryptedPacket);
};

class NET_EXPORT_PRIVATE RetransmittableFrames {
 public:
  RetransmittableFrames();
  ~RetransmittableFrames();

  
  
  
  const QuicFrame& AddStreamFrame(QuicStreamFrame* stream_frame);
  
  const QuicFrame& AddNonStreamFrame(const QuicFrame& frame);
  const QuicFrames& frames() const { return frames_; }

  void set_encryption_level(EncryptionLevel level);
  EncryptionLevel encryption_level() const {
    return encryption_level_;
  }

 private:
  QuicFrames frames_;
  EncryptionLevel encryption_level_;
  
  std::vector<std::string*> stream_data_;

  DISALLOW_COPY_AND_ASSIGN(RetransmittableFrames);
};

struct NET_EXPORT_PRIVATE SerializedPacket {
  SerializedPacket(QuicPacketSequenceNumber sequence_number,
                   QuicSequenceNumberLength sequence_number_length,
                   QuicPacket* packet,
                   QuicPacketEntropyHash entropy_hash,
                   RetransmittableFrames* retransmittable_frames);
  ~SerializedPacket();

  QuicPacketSequenceNumber sequence_number;
  QuicSequenceNumberLength sequence_number_length;
  QuicPacket* packet;
  QuicPacketEntropyHash entropy_hash;
  RetransmittableFrames* retransmittable_frames;

  
  std::set<QuicAckNotifier*> notifiers;
};

struct QuicConsumedData {
  QuicConsumedData(size_t bytes_consumed, bool fin_consumed)
      : bytes_consumed(bytes_consumed),
        fin_consumed(fin_consumed) {}
  
  
  
  
  NET_EXPORT_PRIVATE friend std::ostream& operator<<(
      std::ostream& os, const QuicConsumedData& s);

  
  size_t bytes_consumed;

  
  bool fin_consumed;
};

enum WriteStatus {
  WRITE_STATUS_OK,
  WRITE_STATUS_BLOCKED,
  WRITE_STATUS_ERROR,
};

// of bytes written or the error code, depending upon the status.
struct NET_EXPORT_PRIVATE WriteResult {
  WriteResult(WriteStatus status, int bytes_written_or_error_code) :
    status(status), bytes_written(bytes_written_or_error_code) {
  }

  WriteStatus status;
  union {
    int bytes_written;  
    int error_code;  
  };
};

}  

#endif  
