// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_SERVER_H_
#define NET_PROXY_PROXY_SERVER_H_
#pragma once

#include "build/build_config.h"

#if defined(OS_MACOSX)
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <string>
#include "net/base/host_port_pair.h"
#include "net/base/net_export.h"

namespace net {

class NET_EXPORT ProxyServer {
 public:
  
  
  
  enum Scheme {
    SCHEME_INVALID = 1 << 0,
    SCHEME_DIRECT  = 1 << 1,
    SCHEME_HTTP    = 1 << 2,
    SCHEME_SOCKS4  = 1 << 3,
    SCHEME_SOCKS5  = 1 << 4,
    SCHEME_HTTPS   = 1 << 5,
  };

  

  
  ProxyServer() : scheme_(SCHEME_INVALID) {}

  ProxyServer(Scheme scheme, const HostPortPair& host_port_pair);

  bool is_valid() const { return scheme_ != SCHEME_INVALID; }

  
  Scheme scheme() const { return scheme_; }

  
  bool is_direct() const { return scheme_ == SCHEME_DIRECT; }

  
  bool is_http() const { return scheme_ == SCHEME_HTTP; }

  
  bool is_https() const { return scheme_ == SCHEME_HTTPS; }

  
  bool is_socks() const {
    return scheme_ == SCHEME_SOCKS4 || scheme_ == SCHEME_SOCKS5;
  }

  const HostPortPair& host_port_pair() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static ProxyServer FromURI(const std::string& uri, Scheme default_scheme);
  static ProxyServer FromURI(std::string::const_iterator uri_begin,
                             std::string::const_iterator uri_end,
                             Scheme default_scheme);

  
  std::string ToURI() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  static ProxyServer FromPacString(const std::string& pac_string);
  static ProxyServer FromPacString(std::string::const_iterator pac_string_begin,
                                   std::string::const_iterator pac_string_end);

  
  static ProxyServer Direct() {
    return ProxyServer(SCHEME_DIRECT, HostPortPair());
  }

#if defined(OS_MACOSX)
  
  
  
  
  
  static ProxyServer FromDictionary(Scheme scheme,
                                    CFDictionaryRef dict,
                                    CFStringRef host_key,
                                    CFStringRef port_key);
#endif

  
  std::string ToPacString() const;

  
  
  static int GetDefaultPortForScheme(Scheme scheme);

  
  
  
  
  static Scheme GetSchemeFromURI(const std::string& scheme);

  bool operator==(const ProxyServer& other) const {
    return scheme_ == other.scheme_ &&
           host_port_pair_.Equals(other.host_port_pair_);
  }

  
  bool operator<(const ProxyServer& other) const {
    if (scheme_ != other.scheme_)
      return scheme_ < other.scheme_;
    return host_port_pair_ < other.host_port_pair_;
  }

 private:
  
  
  static ProxyServer FromSchemeHostAndPort(
      Scheme scheme,
      std::string::const_iterator host_and_port_begin,
      std::string::const_iterator host_and_port_end);

  Scheme scheme_;
  HostPortPair host_port_pair_;
};

typedef std::pair<HostPortPair, ProxyServer> HostPortProxyPair;

}  

#endif  
