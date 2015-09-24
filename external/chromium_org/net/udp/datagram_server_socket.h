// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_UDP_DATAGRAM_SERVER_SOCKET_H_
#define NET_UDP_DATAGRAM_SERVER_SOCKET_H_

#include "net/base/completion_callback.h"
#include "net/base/net_util.h"
#include "net/udp/datagram_socket.h"

namespace net {

class IPEndPoint;
class IOBuffer;

class NET_EXPORT DatagramServerSocket : public DatagramSocket {
 public:
  virtual ~DatagramServerSocket() {}

  
  
  virtual int Listen(const IPEndPoint& address) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int RecvFrom(IOBuffer* buf,
                       int buf_len,
                       IPEndPoint* address,
                       const CompletionCallback& callback) = 0;

  
  
  
  
  
  
  
  
  
  virtual int SendTo(IOBuffer* buf,
                     int buf_len,
                     const IPEndPoint& address,
                     const CompletionCallback& callback) = 0;

  
  virtual bool SetReceiveBufferSize(int32 size) = 0;

  
  virtual bool SetSendBufferSize(int32 size) = 0;

  
  
  virtual void AllowAddressReuse() = 0;

  
  
  virtual void AllowBroadcast() = 0;

  
  
  virtual int JoinGroup(const IPAddressNumber& group_address) const = 0;

  
  
  
  
  
  virtual int LeaveGroup(const IPAddressNumber& group_address) const = 0;

  
  
  
  
  virtual int SetMulticastInterface(uint32 interface_index) = 0;

  
  
  
  
  
  virtual int SetMulticastTimeToLive(int time_to_live) = 0;

  
  
  
  
  
  virtual int SetMulticastLoopbackMode(bool loopback) = 0;

  
  
  virtual int SetDiffServCodePoint(DiffServCodePoint dscp) = 0;
};

}  

#endif  
