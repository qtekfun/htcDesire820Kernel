// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_UDP_DATAGRAM_SOCKET_H_
#define NET_UDP_DATAGRAM_SOCKET_H_

#include "net/base/net_export.h"

namespace net {

class BoundNetLog;
class IPEndPoint;

class NET_EXPORT_PRIVATE DatagramSocket {
 public:
  
  enum BindType {
    RANDOM_BIND,
    DEFAULT_BIND,
  };

  virtual ~DatagramSocket() {}

  
  virtual void Close() = 0;

  
  virtual int GetPeerAddress(IPEndPoint* address) const = 0;

  
  
  virtual int GetLocalAddress(IPEndPoint* address) const = 0;

  
  virtual const BoundNetLog& NetLog() const = 0;
};

}  

#endif  
