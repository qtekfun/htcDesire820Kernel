// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_TCP_LISTEN_SOCKET_H_
#define NET_SOCKET_TCP_LISTEN_SOCKET_H_

#include <string>

#include "base/basictypes.h"
#include "net/base/net_export.h"
#include "net/socket/socket_descriptor.h"
#include "net/socket/stream_listen_socket.h"

namespace net {

class NET_EXPORT TCPListenSocket : public StreamListenSocket {
 public:
  virtual ~TCPListenSocket();
  
  
  static scoped_ptr<TCPListenSocket> CreateAndListen(
      const std::string& ip, int port, StreamListenSocket::Delegate* del);

  
  static SocketDescriptor CreateAndBind(const std::string& ip, int port);

  
  static SocketDescriptor CreateAndBindAnyPort(const std::string& ip,
                                               int* port);

 protected:
  TCPListenSocket(SocketDescriptor s, StreamListenSocket::Delegate* del);

  
  virtual void Accept() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(TCPListenSocket);
};

class NET_EXPORT TCPListenSocketFactory : public StreamListenSocketFactory {
 public:
  TCPListenSocketFactory(const std::string& ip, int port);
  virtual ~TCPListenSocketFactory();

  
  virtual scoped_ptr<StreamListenSocket> CreateAndListen(
      StreamListenSocket::Delegate* delegate) const OVERRIDE;

 private:
  const std::string ip_;
  const int port_;

  DISALLOW_COPY_AND_ASSIGN(TCPListenSocketFactory);
};

}  

#endif  
