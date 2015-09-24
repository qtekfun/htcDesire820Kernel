// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_HOST_PORT_PAIR_H_
#define NET_BASE_HOST_PORT_PAIR_H_
#pragma once

#include <string>
#include "base/basictypes.h"
#include "net/base/net_export.h"

struct addrinfo;
class GURL;

namespace net {

class NET_EXPORT HostPortPair {
 public:
  HostPortPair();
  
  HostPortPair(const std::string& in_host, uint16 in_port);

  
  static HostPortPair FromURL(const GURL& url);

  
  static HostPortPair FromAddrInfo(const struct addrinfo* ai);

  
  
  bool operator<(const HostPortPair& other) const {
    if (port_ != other.port_)
      return port_ < other.port_;
    return host_ < other.host_;
  }

  
  bool Equals(const HostPortPair& other) const {
    return host_ == other.host_ && port_ == other.port_;
  }

  const std::string& host() const {
    return host_;
  }

  uint16 port() const {
    return port_;
  }

  void set_host(const std::string& in_host) {
    host_ = in_host;
  }

  void set_port(uint16 in_port) {
    port_ = in_port;
  }

  
  
  std::string ToString() const;

  
  std::string HostForURL() const;

 private:
  
  
  std::string host_;
  uint16 port_;
};

}  

#endif  
