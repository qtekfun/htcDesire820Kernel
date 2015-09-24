// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_IP_ENDPOINT_H_
#define NET_BASE_IP_ENDPOINT_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "net/base/address_family.h"
#include "net/base/net_export.h"
#include "net/base/net_util.h"

struct sockaddr;

namespace net {

class NET_EXPORT IPEndPoint {
 public:
  IPEndPoint();
  virtual ~IPEndPoint();
  IPEndPoint(const IPAddressNumber& address, int port);
  IPEndPoint(const IPEndPoint& endpoint);

  const IPAddressNumber& address() const { return address_; }
  int port() const { return port_; }

  
  AddressFamily GetFamily() const;

  
  int GetSockAddrFamily() const;

  
  
  
  
  
  
  
  bool ToSockAddr(struct sockaddr* address, socklen_t* address_length) const
      WARN_UNUSED_RESULT;

  
  
  
  
  bool FromSockAddr(const struct sockaddr* address, socklen_t address_length)
      WARN_UNUSED_RESULT;

  
  
  
  std::string ToString() const;

  
  std::string ToStringWithoutPort() const;

  bool operator<(const IPEndPoint& that) const;
  bool operator==(const IPEndPoint& that) const;

 private:
  IPAddressNumber address_;
  int port_;
};

}  

#endif  
