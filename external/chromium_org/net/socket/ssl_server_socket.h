// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SSL_SERVER_SOCKET_H_
#define NET_SOCKET_SSL_SERVER_SOCKET_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/completion_callback.h"
#include "net/base/net_export.h"
#include "net/socket/ssl_socket.h"
#include "net/socket/stream_socket.h"

namespace crypto {
class RSAPrivateKey;
}  

namespace net {

struct SSLConfig;
class X509Certificate;

class SSLServerSocket : public SSLSocket {
 public:
  virtual ~SSLServerSocket() {}

  
  
  
  
  virtual int Handshake(const CompletionCallback& callback) = 0;
};

NET_EXPORT void EnableSSLServerSockets();

NET_EXPORT scoped_ptr<SSLServerSocket> CreateSSLServerSocket(
    scoped_ptr<StreamSocket> socket,
    X509Certificate* certificate,
    crypto::RSAPrivateKey* key,
    const SSLConfig& ssl_config);

}  

#endif  
