// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_PACKET_WRITER_H_
#define NET_QUIC_QUIC_PACKET_WRITER_H_

#include "net/base/ip_endpoint.h"
#include "net/quic/quic_protocol.h"

namespace net {

class QuicBlockedWriterInterface;
struct WriteResult;

class NET_EXPORT_PRIVATE QuicPacketWriter {
 public:
  virtual ~QuicPacketWriter() {}

  
  // status is WRITE_STATUS_OK and bytes_written is populated. If the write
  
  
  virtual WriteResult WritePacket(
      const char* buffer, size_t buf_len,
      const net::IPAddressNumber& self_address,
      const net::IPEndPoint& peer_address,
      QuicBlockedWriterInterface* blocked_writer) = 0;

  
  
  
  virtual bool IsWriteBlockedDataBuffered() const = 0;
};

}  

#endif  
