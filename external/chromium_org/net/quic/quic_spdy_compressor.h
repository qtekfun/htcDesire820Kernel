// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_SPDY_COMPRESSOR_H_
#define NET_QUIC_QUIC_SPDY_COMPRESSOR_H_

#include <string>

#include "net/base/net_export.h"
#include "net/quic/quic_protocol.h"
#include "net/spdy/spdy_framer.h"

namespace net {

class NET_EXPORT_PRIVATE QuicSpdyCompressor {
 public:
  QuicSpdyCompressor();
  ~QuicSpdyCompressor();

  
  std::string CompressHeaders(const SpdyHeaderBlock& headers);

  
  
  std::string CompressHeadersWithPriority(QuicPriority priority,
                                          const SpdyHeaderBlock& headers);

 private:
  std::string CompressHeadersInternal(QuicPriority priority,
                                      const SpdyHeaderBlock& headers,
                                      bool write_priority);

  SpdyFramer spdy_framer_;
  QuicHeaderId header_sequence_id_;

  DISALLOW_COPY_AND_ASSIGN(QuicSpdyCompressor);
};

}  

#endif  
