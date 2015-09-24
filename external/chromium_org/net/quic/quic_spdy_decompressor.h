// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_SPDY_DECOMPRESSOR_H_
#define NET_QUIC_QUIC_SPDY_DECOMPRESSOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "net/base/net_export.h"
#include "net/quic/quic_protocol.h"
#include "net/spdy/spdy_framer.h"

namespace net {

class SpdyFramerVisitor;

class NET_EXPORT_PRIVATE QuicSpdyDecompressor {
 public:
  
  
  class NET_EXPORT_PRIVATE Visitor {
   public:
    virtual ~Visitor() {}
    virtual bool OnDecompressedData(base::StringPiece data) = 0;
    virtual void OnDecompressionError() = 0;
  };

  QuicSpdyDecompressor();
  ~QuicSpdyDecompressor();

  
  
  
  size_t DecompressData(base::StringPiece data, Visitor* visitor);

  QuicHeaderId current_header_id() { return current_header_id_; }

 private:
  void ResetForNextHeaders();

  SpdyFramer spdy_framer_;
  scoped_ptr<SpdyFramerVisitor> spdy_visitor_;
  
  QuicHeaderId current_header_id_;
  
  bool has_current_compressed_size_;
  
  uint32 current_compressed_size_;
  // Buffer into which the partial compressed size is written until
  
  std::string compressed_size_buffer_;
  
  
  uint32 compressed_bytes_consumed_;
  DISALLOW_COPY_AND_ASSIGN(QuicSpdyDecompressor);
};

}  

#endif  
