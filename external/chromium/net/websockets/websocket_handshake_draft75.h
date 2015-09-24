// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_WEBSOCKETS_WEBSOCKET_HANDSHAKE_DRAFT75_H_
#define NET_WEBSOCKETS_WEBSOCKET_HANDSHAKE_DRAFT75_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "googleurl/src/gurl.h"
#include "net/websockets/websocket_handshake.h"

namespace net {

class HttpResponseHeaders;

class WebSocketHandshakeDraft75 : public WebSocketHandshake {
 public:
  static const int kWebSocketPort;
  static const int kSecureWebSocketPort;
  static const char kServerHandshakeHeader[];
  static const size_t kServerHandshakeHeaderLength;
  static const char kUpgradeHeader[];
  static const size_t kUpgradeHeaderLength;
  static const char kConnectionHeader[];
  static const size_t kConnectionHeaderLength;

  WebSocketHandshakeDraft75(const GURL& url,
                            const std::string& origin,
                            const std::string& location,
                            const std::string& protocol);
  virtual ~WebSocketHandshakeDraft75();

  
  virtual std::string CreateClientHandshakeMessage();

  
  
  
  
  
  virtual int ReadServerHandshake(const char* data, size_t len);

 private:
  
  
  
  
  virtual bool ProcessHeaders(const HttpResponseHeaders& headers);

  
  
  
  
  virtual bool CheckResponseHeaders() const;

  DISALLOW_COPY_AND_ASSIGN(WebSocketHandshakeDraft75);
};

}  

#endif  
