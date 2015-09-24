// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_QUIC_QUIC_SPDY_SERVER_STREAM_H_
#define NET_TOOLS_QUIC_QUIC_SPDY_SERVER_STREAM_H_

#include <string>

#include "net/base/io_buffer.h"
#include "net/quic/quic_data_stream.h"
#include "net/quic/quic_protocol.h"
#include "net/tools/balsa/balsa_headers.h"

namespace net {

class QuicSession;

namespace tools {

namespace test {
class QuicSpdyServerStreamPeer;
}  

class QuicSpdyServerStream : public QuicDataStream {
 public:
  QuicSpdyServerStream(QuicStreamId id, QuicSession* session);
  virtual ~QuicSpdyServerStream();

  
  
  virtual uint32 ProcessData(const char* data, uint32 data_len) OVERRIDE;
  virtual void OnFinRead() OVERRIDE;

  int ParseRequestHeaders();

 private:
  friend class test::QuicSpdyServerStreamPeer;

  
  
  void SendResponse();

  
  
  void SendErrorResponse();

  void SendHeadersAndBody(const BalsaHeaders& response_headers,
                          base::StringPiece body);

  BalsaHeaders headers_;
  string body_;

  
  scoped_refptr<GrowableIOBuffer> read_buf_;
  bool request_headers_received_;
};

}  
}  

#endif  
