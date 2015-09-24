// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_RELIABLE_QUIC_STREAM_H_
#define NET_QUIC_RELIABLE_QUIC_STREAM_H_

#include <sys/types.h>

#include <list>

#include "base/strings/string_piece.h"
#include "net/base/iovec.h"
#include "net/base/net_export.h"
#include "net/quic/quic_ack_notifier.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_spdy_compressor.h"
#include "net/quic/quic_stream_sequencer.h"

namespace net {

namespace test {
class ReliableQuicStreamPeer;
}  

class IPEndPoint;
class QuicSession;
class SSLInfo;

class NET_EXPORT_PRIVATE ReliableQuicStream {
 public:
  ReliableQuicStream(QuicStreamId id,
                     QuicSession* session);

  virtual ~ReliableQuicStream();

  bool WillAcceptStreamFrame(const QuicStreamFrame& frame) const;

  
  
  
  virtual bool OnStreamFrame(const QuicStreamFrame& frame);

  
  
  virtual void OnCanWrite();

  
  virtual void OnClose();

  
  virtual void OnStreamReset(QuicRstStreamErrorCode error);

  
  
  
  virtual void OnConnectionClosed(QuicErrorCode error, bool from_peer);

  
  virtual void OnFinRead();

  virtual uint32 ProcessRawData(const char* data, uint32 data_len) = 0;

  
  virtual void Reset(QuicRstStreamErrorCode error);

  
  virtual void CloseConnection(QuicErrorCode error);
  virtual void CloseConnectionWithDetails(QuicErrorCode error,
                                          const string& details);

  
  
  virtual QuicPriority EffectivePriority() const = 0;

  QuicStreamId id() const { return id_; }

  QuicRstStreamErrorCode stream_error() const { return stream_error_; }
  QuicErrorCode connection_error() const { return connection_error_; }

  bool read_side_closed() const { return read_side_closed_; }
  bool write_side_closed() const { return write_side_closed_; }

  uint64 stream_bytes_read() { return stream_bytes_read_; }
  uint64 stream_bytes_written() { return stream_bytes_written_; }

  QuicVersion version();

 protected:
  
  
  void WriteOrBufferData(base::StringPiece data, bool fin);

  
  
  
  
  
  QuicConsumedData WritevData(
      const struct iovec* iov,
      int iov_count,
      bool fin,
      QuicAckNotifier::DelegateInterface* ack_notifier_delegate);

  
  virtual void CloseReadSide();

  
  void CloseWriteSide();

  bool HasBufferedData();

  bool fin_buffered() { return fin_buffered_; }

  QuicSession* session() { return session_; }

  const QuicStreamSequencer* sequencer() const { return &sequencer_; }
  QuicStreamSequencer* sequencer() { return &sequencer_; }

 private:
  friend class test::ReliableQuicStreamPeer;
  friend class QuicStreamUtils;

  std::list<string> queued_data_;

  QuicStreamSequencer sequencer_;
  QuicStreamId id_;
  QuicSession* session_;
  // Bytes read and written refer to payload bytes only: they do not include
  
  uint64 stream_bytes_read_;
  uint64 stream_bytes_written_;

  
  
  QuicRstStreamErrorCode stream_error_;
  
  
  
  QuicErrorCode connection_error_;

  
  bool read_side_closed_;
  
  bool write_side_closed_;

  bool fin_buffered_;
  bool fin_sent_;

  
  bool is_server_;

  DISALLOW_COPY_AND_ASSIGN(ReliableQuicStream);
};

}  

#endif  
