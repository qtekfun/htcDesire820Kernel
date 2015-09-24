// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_CLIENT_SOCKET_FACTORY_H_
#define NET_SOCKET_CLIENT_SOCKET_FACTORY_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/base/net_log.h"
#include "net/base/rand_callback.h"
#include "net/udp/datagram_socket.h"

namespace net {

class AddressList;
class ClientSocketHandle;
class DatagramClientSocket;
class HostPortPair;
class SSLClientSocket;
struct SSLClientSocketContext;
struct SSLConfig;
class StreamSocket;

class NET_EXPORT ClientSocketFactory {
 public:
  virtual ~ClientSocketFactory() {}

  
  
  virtual scoped_ptr<DatagramClientSocket> CreateDatagramClientSocket(
      DatagramSocket::BindType bind_type,
      const RandIntCallback& rand_int_cb,
      NetLog* net_log,
      const NetLog::Source& source) = 0;

  virtual scoped_ptr<StreamSocket> CreateTransportClientSocket(
      const AddressList& addresses,
      NetLog* net_log,
      const NetLog::Source& source) = 0;

  
  
  
  virtual scoped_ptr<SSLClientSocket> CreateSSLClientSocket(
      scoped_ptr<ClientSocketHandle> transport_socket,
      const HostPortPair& host_and_port,
      const SSLConfig& ssl_config,
      const SSLClientSocketContext& context) = 0;

  
  virtual void ClearSSLSessionCache() = 0;

  
  static ClientSocketFactory* GetDefaultFactory();
};

}  

#endif  
