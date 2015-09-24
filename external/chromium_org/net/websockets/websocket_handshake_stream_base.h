// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_HANDSHAKE_STREAM_BASE_H_
#define NET_WEBSOCKETS_WEBSOCKET_HANDSHAKE_STREAM_BASE_H_


#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/supports_user_data.h"
#include "net/http/http_stream_base.h"
#include "net/url_request/websocket_handshake_userdata_key.h"
#include "net/websockets/websocket_stream.h"

namespace net {

class ClientSocketHandle;
class SpdySession;

class NET_EXPORT WebSocketHandshakeStreamBase : public HttpStreamBase {
 public:
  
  
  
  class NET_EXPORT_PRIVATE CreateHelper : public base::SupportsUserData::Data {
   public:
    
    
    
    static const void* DataKey() { return kWebSocketHandshakeUserDataKey; }

    virtual ~CreateHelper() {}

    
    
    
    
    virtual WebSocketHandshakeStreamBase* CreateBasicStream(
        scoped_ptr<ClientSocketHandle> connection,
        bool using_proxy) = 0;

    
    virtual WebSocketHandshakeStreamBase* CreateSpdyStream(
        const base::WeakPtr<SpdySession>& session,
        bool use_relative_url) = 0;
  };

  
  
  virtual ~WebSocketHandshakeStreamBase() {}

  
  
  
  
  virtual scoped_ptr<WebSocketStream> Upgrade() = 0;

 protected:
  
  WebSocketHandshakeStreamBase() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(WebSocketHandshakeStreamBase);
};

}  

#endif  
