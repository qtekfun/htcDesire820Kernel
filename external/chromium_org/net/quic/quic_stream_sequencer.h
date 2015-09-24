// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_STREAM_SEQUENCER_H_
#define NET_QUIC_QUIC_STREAM_SEQUENCER_H_

#include <map>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/iovec.h"
#include "net/quic/quic_protocol.h"

using std::map;
using std::string;

namespace net {

namespace test {
class QuicStreamSequencerPeer;
}  

class QuicSession;
class ReliableQuicStream;

class NET_EXPORT_PRIVATE QuicStreamSequencer {
 public:
  explicit QuicStreamSequencer(ReliableQuicStream* quic_stream);
  QuicStreamSequencer(size_t max_frame_memory,
                      ReliableQuicStream* quic_stream);

  virtual ~QuicStreamSequencer();

  
  bool WillAcceptStreamFrame(const QuicStreamFrame& frame) const;

  
  
  
  
  
  
  
  bool OnStreamFrame(const QuicStreamFrame& frame);

  
  

  
  
  int GetReadableRegions(iovec* iov, size_t iov_len);

  
  
  int Readv(const struct iovec* iov, size_t iov_len);

  
  
  void MarkConsumed(size_t num_bytes);

  
  bool HasBytesToRead() const;

  
  bool IsClosed() const;

  
  bool IsDuplicate(const QuicStreamFrame& frame) const;

  
  
  void FlushBufferedFrames();

 private:
  friend class test::QuicStreamSequencerPeer;

  
  typedef map<QuicStreamOffset, string> FrameMap;

  
  void CloseStreamAtOffset(QuicStreamOffset offset);

  bool MaybeCloseStream();

  ReliableQuicStream* stream_;  
  QuicStreamOffset num_bytes_consumed_;  
  FrameMap frames_;  
  size_t max_frame_memory_;  
  
  
  QuicStreamOffset close_offset_;
};

}  

#endif  
