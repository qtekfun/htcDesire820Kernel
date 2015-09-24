// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_UDP_DATAGRAM_CLIENT_SOCKET_H_
#define NET_UDP_DATAGRAM_CLIENT_SOCKET_H_

#include "net/socket/socket.h"
#include "net/udp/datagram_socket.h"

namespace net {

class IPEndPoint;

class NET_EXPORT_PRIVATE DatagramClientSocket : public DatagramSocket,
                                                public Socket {
 public:
  virtual ~DatagramClientSocket() {}

  
  
  virtual int Connect(const IPEndPoint& address) = 0;
};

}  

#endif  
