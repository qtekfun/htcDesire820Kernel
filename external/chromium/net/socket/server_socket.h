// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_SERVER_SOCKET_H_
#define NET_SOCKET_SERVER_SOCKET_H_

#include "base/scoped_ptr.h"
#include "net/base/completion_callback.h"

namespace net {

class ClientSocket;
class IPEndPoint;

class ServerSocket {
 public:
  ServerSocket() { }
  virtual ~ServerSocket() { }

  
  
  virtual int Listen(const net::IPEndPoint& address, int backlog) = 0;

  
  virtual int GetLocalAddress(IPEndPoint* address) const = 0;

  
  
  virtual int Accept(scoped_ptr<ClientSocket>* socket,
                     CompletionCallback* callback) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ServerSocket);
};

}  

#endif  
