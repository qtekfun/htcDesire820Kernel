// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_UDP_DATAGRAM_SOCKET_H_
#define NET_UDP_DATAGRAM_SOCKET_H_
#pragma once

namespace net {

class IPEndPoint;

class DatagramSocket {
 public:
  virtual ~DatagramSocket() {}

  
  virtual void Close() = 0;

  
  virtual int GetPeerAddress(IPEndPoint* address) const = 0;

  
  
  virtual int GetLocalAddress(IPEndPoint* address) const = 0;
};

}  

#endif  
