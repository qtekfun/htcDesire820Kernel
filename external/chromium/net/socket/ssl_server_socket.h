// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SSL_SERVER_SOCKET_H_
#define NET_SOCKET_SSL_SERVER_SOCKET_H_

#include "base/basictypes.h"
#include "net/base/completion_callback.h"
#include "net/socket/socket.h"

namespace crypto {
class RSAPrivateKey;
}  

namespace net {

class IOBuffer;
struct SSLConfig;
class X509Certificate;

class SSLServerSocket : public Socket {
 public:
  virtual ~SSLServerSocket() {}

  
  
  
  
  
  
  virtual int Accept(CompletionCallback* callback) = 0;
};

SSLServerSocket* CreateSSLServerSocket(
    Socket* socket, X509Certificate* certificate, crypto::RSAPrivateKey* key,
    const SSLConfig& ssl_config);

}  

#endif  
