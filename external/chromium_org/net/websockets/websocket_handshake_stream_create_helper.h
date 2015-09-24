// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_HANDSHAKE_STREAM_CREATE_HELPER_H_
#define NET_WEBSOCKETS_WEBSOCKET_HANDSHAKE_STREAM_CREATE_HELPER_H_

#include <string>
#include <vector>

#include "net/base/net_export.h"
#include "net/websockets/websocket_handshake_stream_base.h"

namespace net {

class NET_EXPORT_PRIVATE WebSocketHandshakeStreamCreateHelper
    : public WebSocketHandshakeStreamBase::CreateHelper {
 public:
  explicit WebSocketHandshakeStreamCreateHelper(
      const std::vector<std::string>& requested_subprotocols);

  virtual ~WebSocketHandshakeStreamCreateHelper();

  

  
  virtual WebSocketHandshakeStreamBase* CreateBasicStream(
      scoped_ptr<ClientSocketHandle> connection,
      bool using_proxy) OVERRIDE;

  
  virtual WebSocketHandshakeStreamBase* CreateSpdyStream(
      const base::WeakPtr<SpdySession>& session,
      bool use_relative_url) OVERRIDE;

  
  
  
  WebSocketHandshakeStreamBase* stream() { return stream_; }

 private:
  const std::vector<std::string> requested_subprotocols_;

  
  
  
  
  WebSocketHandshakeStreamBase* stream_;

  DISALLOW_COPY_AND_ASSIGN(WebSocketHandshakeStreamCreateHelper);
};

}  

#endif  
