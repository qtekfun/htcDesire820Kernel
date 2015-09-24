// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_UDP_DATAGRAM_SERVER_SOCKET_H_
#define NET_UDP_DATAGRAM_SERVER_SOCKET_H_
#pragma once

#include "net/base/completion_callback.h"
#include "net/udp/datagram_socket.h"

namespace net {

class IPEndPoint;
class IOBuffer;

class DatagramServerSocket : public DatagramSocket {
 public:
  virtual ~DatagramServerSocket() {}

  
  
  virtual int Listen(const IPEndPoint& address) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int RecvFrom(IOBuffer* buf,
                       int buf_len,
                       IPEndPoint* address,
                       CompletionCallback* callback) = 0;

  
  
  
  
  
  
  
  
  
  virtual int SendTo(IOBuffer* buf,
                     int buf_len,
                     const IPEndPoint& address,
                     CompletionCallback* callback) = 0;
};

}  

#endif  
