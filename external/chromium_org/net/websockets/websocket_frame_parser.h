// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_FRAME_PARSER_H_
#define NET_WEBSOCKETS_WEBSOCKET_FRAME_PARSER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "net/base/net_export.h"
#include "net/websockets/websocket_errors.h"
#include "net/websockets/websocket_frame.h"

namespace net {


class NET_EXPORT WebSocketFrameParser {
 public:
  WebSocketFrameParser();
  ~WebSocketFrameParser();

  
  
  
  
  
  
  
  
  
  bool Decode(const char* data,
              size_t length,
              ScopedVector<WebSocketFrameChunk>* frame_chunks);

  
  
  
  
  WebSocketError websocket_error() const { return websocket_error_; }

 private:
  
  
  
  
  
  
  void DecodeFrameHeader();

  
  
  
  
  
  
  scoped_ptr<WebSocketFrameChunk> DecodeFramePayload(bool first_chunk);

  
  std::vector<char> buffer_;

  
  size_t current_read_pos_;

  
  
  scoped_ptr<WebSocketFrameHeader> current_frame_header_;
  WebSocketMaskingKey masking_key_;

  
  uint64 frame_offset_;

  WebSocketError websocket_error_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketFrameParser);
};

}  

#endif  
