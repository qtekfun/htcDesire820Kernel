// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_TOOLS_HTTP_LISTEN_SOCKET_H_
#define NET_BASE_TOOLS_HTTP_LISTEN_SOCKET_H_
#pragma once

#include "base/message_loop.h"
#include "net/base/listen_socket.h"

class HttpServerRequestInfo;
class HttpServerResponseInfo;

class HttpListenSocket : public ListenSocket,
                         public ListenSocket::ListenSocketDelegate {
 public:
  class Delegate {
   public:
    virtual void OnRequest(HttpListenSocket* connection,
                           HttpServerRequestInfo* info) = 0;

   protected:
    virtual ~Delegate() {}
  };

  static HttpListenSocket* Listen(const std::string& ip, int port,
                                  HttpListenSocket::Delegate* delegate);

  virtual void Listen();
  virtual void Accept();

  
  
  void Respond(HttpServerResponseInfo* info, std::string& data);

  
  virtual void DidAccept(ListenSocket* server, ListenSocket* connection);
  virtual void DidRead(ListenSocket* connection, const char* data, int len);
  virtual void DidClose(ListenSocket* sock);

 private:
  friend class base::RefCountedThreadSafe<ListenSocket>;

  static const int kReadBufSize = 16 * 1024;
  HttpListenSocket(SOCKET s, HttpListenSocket::Delegate* del);
  virtual ~HttpListenSocket();

  
  
  
  HttpServerRequestInfo* ParseHeaders();

  HttpListenSocket::Delegate* delegate_;
  std::string recv_data_;

  DISALLOW_COPY_AND_ASSIGN(HttpListenSocket);
};

#endif 
