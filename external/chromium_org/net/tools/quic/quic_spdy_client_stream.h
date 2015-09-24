// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_QUIC_QUIC_SPDY_CLIENT_STREAM_H_
#define NET_TOOLS_QUIC_QUIC_SPDY_CLIENT_STREAM_H_

#include <sys/types.h>
#include <string>

#include "base/strings/string_piece.h"
#include "net/base/io_buffer.h"
#include "net/quic/quic_data_stream.h"
#include "net/quic/quic_protocol.h"
#include "net/tools/balsa/balsa_frame.h"
#include "net/tools/balsa/balsa_headers.h"

namespace net {

namespace tools {

class QuicClientSession;

class QuicSpdyClientStream : public QuicDataStream {
 public:
  QuicSpdyClientStream(QuicStreamId id, QuicClientSession* session);
  virtual ~QuicSpdyClientStream();

  
  
  
  virtual bool OnStreamFrame(const QuicStreamFrame& frame) OVERRIDE;

  
  
  virtual uint32 ProcessData(const char* data, uint32 data_len) OVERRIDE;

  virtual void OnFinRead() OVERRIDE;

  
  
  ssize_t SendRequest(const BalsaHeaders& headers,
                      base::StringPiece body,
                      bool fin);

  
  void SendBody(const std::string& data, bool fin);

  
  const std::string& data() { return data_; }

  
  const BalsaHeaders& headers() { return headers_; }

  
  
  using QuicDataStream::set_priority;

 private:
  int ParseResponseHeaders();

  BalsaHeaders headers_;
  std::string data_;

  scoped_refptr<GrowableIOBuffer> read_buf_;
  bool response_headers_received_;
};

}  
}  

#endif  
