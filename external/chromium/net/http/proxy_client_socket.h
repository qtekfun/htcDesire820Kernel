// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_HTTP_PROXY_CLIENT_SOCKET_H_
#define NET_HTTP_PROXY_CLIENT_SOCKET_H_
#pragma once

#include "net/socket/client_socket.h"

namespace net {

class HttpStream;
class HttpResponseInfo;

class ProxyClientSocket : public ClientSocket {
 public:
  ProxyClientSocket() {}
  virtual ~ProxyClientSocket() {}

  
  
  virtual const HttpResponseInfo* GetConnectResponseInfo() const = 0;

  
  
  virtual HttpStream* CreateConnectResponseStream() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ProxyClientSocket);
};

}  

#endif  
