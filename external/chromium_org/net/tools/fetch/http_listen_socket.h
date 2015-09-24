// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_TOOLS_HTTP_LISTEN_SOCKET_H_
#define NET_BASE_TOOLS_HTTP_LISTEN_SOCKET_H_

#include <set>

#include "base/message_loop/message_loop.h"
#include "net/socket/stream_listen_socket.h"
#include "net/socket/tcp_listen_socket.h"

class HttpServerRequestInfo;
class HttpServerResponseInfo;

class HttpListenSocket : public net::TCPListenSocket,
                         public net::StreamListenSocket::Delegate {
 public:
  class Delegate {
   public:
    virtual void OnRequest(HttpListenSocket* connection,
                           HttpServerRequestInfo* info) = 0;

   protected:
    virtual ~Delegate() {}
  };

  virtual ~HttpListenSocket();

  static scoped_ptr<HttpListenSocket> CreateAndListen(
      const std::string& ip, int port, HttpListenSocket::Delegate* delegate);

  
  
  void Respond(HttpServerResponseInfo* info, std::string& data);

  
  virtual void DidAccept(
      net::StreamListenSocket* server,
      scoped_ptr<net::StreamListenSocket> connection) OVERRIDE;
  virtual void DidRead(net::StreamListenSocket* connection,
                       const char* data, int len) OVERRIDE;
  virtual void DidClose(net::StreamListenSocket* sock) OVERRIDE;

 protected:
  
  virtual void Accept() OVERRIDE;

 private:
  static const int kReadBufSize = 16 * 1024;

  
  HttpListenSocket(net::SocketDescriptor s, HttpListenSocket::Delegate* del);

  
  
  
  HttpServerRequestInfo* ParseHeaders();

  HttpListenSocket::Delegate* const delegate_;
  std::string recv_data_;

  std::set<StreamListenSocket*> connections_;

  DISALLOW_COPY_AND_ASSIGN(HttpListenSocket);
};

#endif 
