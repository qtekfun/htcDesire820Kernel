// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_WEBSOCKET_TEST_UTIL_H_
#define NET_SPDY_SPDY_WEBSOCKET_TEST_UTIL_H_

#include "net/base/request_priority.h"
#include "net/spdy/spdy_header_block.h"
#include "net/spdy/spdy_protocol.h"
#include "net/spdy/spdy_test_util_common.h"

namespace net {

class SpdyWebSocketTestUtil {
 public:
  explicit SpdyWebSocketTestUtil(NextProto protocol);

  
  
  std::string GetHeader(const SpdyHeaderBlock& headers,
                        const std::string& key) const;

  
  
  void SetHeader(const std::string& key,
                 const std::string& value,
                 SpdyHeaderBlock* headers) const;

  
  
  SpdyFrame* ConstructSpdyWebSocketSynStream(int stream_id,
                                             const char* path,
                                             const char* host,
                                             const char* origin);

  
  
  SpdyFrame* ConstructSpdyWebSocketSynReply(int stream_id);

  
  SpdyFrame* ConstructSpdyWebSocketHandshakeRequestFrame(
      scoped_ptr<SpdyHeaderBlock> headers,
      SpdyStreamId stream_id,
      RequestPriority request_priority);

  
  SpdyFrame* ConstructSpdyWebSocketHandshakeResponseFrame(
      scoped_ptr<SpdyHeaderBlock> headers,
      SpdyStreamId stream_id,
      RequestPriority request_priority);

  
  SpdyFrame* ConstructSpdyWebSocketHeadersFrame(int stream_id,
                                                const char* length,
                                                bool fin);

  
  SpdyFrame* ConstructSpdyWebSocketDataFrame(const char* data,
                                             int len,
                                             SpdyStreamId stream_id,
                                             bool fin);

  
  SpdyFrame* ConstructSpdySettings(const SettingsMap& settings) const;
  SpdyMajorVersion spdy_version() const;

 private:
  
  std::string GetHeaderKey(const std::string& key) const;

  SpdyTestUtil spdy_util_;
};

}  

#endif  
