// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_IP_ENDPOINT_H_
#define NET_BASE_IP_ENDPOINT_H_
#pragma once

#include "base/basictypes.h"
#include "net/base/net_util.h"

struct sockaddr;

namespace net {

class IPEndPoint {
 public:
  IPEndPoint();
  virtual ~IPEndPoint();
  IPEndPoint(const IPAddressNumber& address, int port);
  IPEndPoint(const IPEndPoint& endpoint);

  const IPAddressNumber& address() const { return address_; }
  int port() const { return port_; }

  
  int GetFamily() const;

  
  
  
  
  
  
  
  bool ToSockAddr(struct sockaddr* address, size_t* address_length) const;

  
  
  
  
  bool FromSockAddr(const struct sockaddr* address, size_t address_length);

  
  
  
  std::string ToString() const;

  bool operator<(const IPEndPoint& that) const;
  bool operator==(const IPEndPoint& that) const;

 private:
  IPAddressNumber address_;
  int port_;
};

}  

#endif  
