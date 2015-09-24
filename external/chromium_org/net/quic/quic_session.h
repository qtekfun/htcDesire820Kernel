// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_SESSION_H_
#define NET_QUIC_QUIC_SESSION_H_

#include <vector>

#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "net/base/ip_endpoint.h"
#include "net/base/linked_hash_map.h"
#include "net/quic/quic_connection.h"
#include "net/quic/quic_crypto_stream.h"
#include "net/quic/quic_data_stream.h"
#include "net/quic/quic_packet_creator.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_spdy_compressor.h"
#include "net/quic/quic_spdy_decompressor.h"
#include "net/quic/reliable_quic_stream.h"
#include "net/spdy/write_blocked_list.h"

namespace net {

class QuicCryptoStream;
class ReliableQuicStream;
class SSLInfo;
class VisitorShim;

namespace test {
class QuicSessionPeer;
}  

class NET_EXPORT_PRIVATE QuicSession : public QuicConnectionVisitorInterface {
 public:
  
  enum CryptoHandshakeEvent {
    
    
    
    ENCRYPTION_FIRST_ESTABLISHED,
    
    
    
    
    ENCRYPTION_REESTABLISHED,
    
    
    
    HANDSHAKE_CONFIRMED,
  };

  QuicSession(QuicConnection* connection,
              const QuicConfig& config);

  virtual ~QuicSession();

  
  virtual bool OnStreamFrames(
      const std::vector<QuicStreamFrame>& frames) OVERRIDE;
  virtual void OnRstStream(const QuicRstStreamFrame& frame) OVERRIDE;
  virtual void OnGoAway(const QuicGoAwayFrame& frame) OVERRIDE;
  virtual void OnConnectionClosed(QuicErrorCode error, bool from_peer) OVERRIDE;
  virtual void OnSuccessfulVersionNegotiation(
      const QuicVersion& version) OVERRIDE {}
  virtual void OnConfigNegotiated() OVERRIDE;
  
  virtual bool OnCanWrite() OVERRIDE;
  virtual bool HasPendingHandshake() const OVERRIDE;

  
  
  
  
  
  
  
  
  virtual QuicConsumedData WritevData(
      QuicStreamId id,
      const struct iovec* iov,
      int iov_count,
      QuicStreamOffset offset,
      bool fin,
      QuicAckNotifier::DelegateInterface* ack_notifier_delegate);

  
  virtual void SendRstStream(QuicStreamId id, QuicRstStreamErrorCode error);

  
  void SendGoAway(QuicErrorCode error_code, const std::string& reason);

  
  virtual void CloseStream(QuicStreamId stream_id);

  
  
  virtual bool IsEncryptionEstablished();

  
  
  virtual bool IsCryptoHandshakeConfirmed();

  
  
  
  
  
  
  
  
  virtual void OnCryptoHandshakeEvent(CryptoHandshakeEvent event);

  
  virtual void OnCryptoHandshakeMessageSent(
      const CryptoHandshakeMessage& message);

  
  virtual void OnCryptoHandshakeMessageReceived(
      const CryptoHandshakeMessage& message);

  
  
  QuicConfig* config();

  
  
  
  bool IsClosedStream(QuicStreamId id);

  QuicConnection* connection() { return connection_.get(); }
  const QuicConnection* connection() const { return connection_.get(); }
  size_t num_active_requests() const { return stream_map_.size(); }
  const IPEndPoint& peer_address() const {
    return connection_->peer_address();
  }
  QuicGuid guid() const { return connection_->guid(); }

  QuicPacketCreator::Options* options() { return connection()->options(); }

  
  
  virtual size_t GetNumOpenStreams() const;

  void MarkWriteBlocked(QuicStreamId id, QuicPriority priority);

  
  
  bool HasQueuedData() const;

  
  
  void MarkDecompressionBlocked(QuicHeaderId decompression_id,
                                QuicStreamId stream_id);

  bool goaway_received() const {
    return goaway_received_;
  }

  bool goaway_sent() const {
    return goaway_sent_;
  }

  QuicSpdyDecompressor* decompressor() { return &decompressor_; }
  QuicSpdyCompressor* compressor() { return &compressor_; }

  
  virtual bool GetSSLInfo(SSLInfo* ssl_info);

  QuicErrorCode error() const { return error_; }

  bool is_server() const { return connection_->is_server(); }

 protected:
  typedef base::hash_map<QuicStreamId, QuicDataStream*> DataStreamMap;

  
  
  
  virtual QuicDataStream* CreateIncomingDataStream(QuicStreamId id) = 0;

  
  
  virtual QuicDataStream* CreateOutgoingDataStream() = 0;

  
  virtual QuicCryptoStream* GetCryptoStream() = 0;

  
  virtual void ActivateStream(QuicDataStream* stream);

  
  QuicStreamId GetNextStreamId();

  QuicDataStream* GetIncomingReliableStream(QuicStreamId stream_id);

  QuicDataStream* GetDataStream(const QuicStreamId stream_id);

  ReliableQuicStream* GetStream(const QuicStreamId stream_id);

  
  
  
  
  virtual void PostProcessAfterData();

  base::hash_map<QuicStreamId, QuicDataStream*>* streams() {
    return &stream_map_;
  }

  const base::hash_map<QuicStreamId, QuicDataStream*>* streams() const {
    return &stream_map_;
  }

  std::vector<QuicDataStream*>* closed_streams() { return &closed_streams_; }

  size_t get_max_open_streams() const {
    return max_open_streams_;
  }

 private:
  friend class test::QuicSessionPeer;
  friend class VisitorShim;

  
  
  
  
  void CloseStreamInner(QuicStreamId stream_id, bool locally_reset);

  
  
  void AddZombieStream(QuicStreamId stream_id);

  
  
  void CloseZombieStream(QuicStreamId stream_id);

  
  
  void AddPrematurelyClosedStream(QuicStreamId stream_id);

  scoped_ptr<QuicConnection> connection_;

  
  
  
  linked_hash_map<QuicStreamId, bool> prematurely_closed_streams_;

  
  
  
  
  linked_hash_map<QuicStreamId, bool> zombie_streams_;

  
  
  scoped_ptr<VisitorShim> visitor_shim_;

  std::vector<QuicDataStream*> closed_streams_;

  QuicSpdyDecompressor decompressor_;
  QuicSpdyCompressor compressor_;

  QuicConfig config_;

  
  size_t max_open_streams_;

  
  DataStreamMap stream_map_;
  QuicStreamId next_stream_id_;

  
  
  base::hash_set<QuicStreamId> implicitly_created_streams_;

  
  WriteBlockedList<QuicStreamId> write_blocked_streams_;

  
  
  map<uint32, QuicStreamId> decompression_blocked_streams_;

  QuicStreamId largest_peer_created_stream_id_;

  
  QuicErrorCode error_;

  
  bool goaway_received_;
  
  bool goaway_sent_;

  
  bool has_pending_handshake_;

  DISALLOW_COPY_AND_ASSIGN(QuicSession);
};

}  

#endif  
