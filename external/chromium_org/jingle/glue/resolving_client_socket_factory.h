// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_GLUE_RESOLVING_CLIENT_SOCKET_FACTORY_H_
#define JINGLE_GLUE_RESOLVING_CLIENT_SOCKET_FACTORY_H_

#include "base/memory/scoped_ptr.h"

namespace net {
class ClientSocketHandle;
class HostPortPair;
class SSLClientSocket;
class StreamSocket;
}  


namespace jingle_glue {

class ResolvingClientSocketFactory {
 public:
  virtual ~ResolvingClientSocketFactory() { }
  
  virtual scoped_ptr<net::StreamSocket> CreateTransportClientSocket(
      const net::HostPortPair& host_and_port) = 0;

  virtual scoped_ptr<net::SSLClientSocket> CreateSSLClientSocket(
      scoped_ptr<net::ClientSocketHandle> transport_socket,
      const net::HostPortPair& host_and_port) = 0;
};

}  

#endif  
