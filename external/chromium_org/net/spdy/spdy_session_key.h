// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SPDY_SPDY_SESSION_KEY_H_
#define NET_SPDY_SPDY_SESSION_KEY_H_

#include "net/base/privacy_mode.h"
#include "net/proxy/proxy_server.h"

namespace net {

class NET_EXPORT_PRIVATE SpdySessionKey {
 public:
  SpdySessionKey();
  SpdySessionKey(const HostPortPair& host_port_pair,
                 const ProxyServer& proxy_server,
                 PrivacyMode privacy_mode);

  
  SpdySessionKey(const HostPortProxyPair& host_port_proxy_pair,
                 PrivacyMode privacy_mode);

  ~SpdySessionKey();

  
  bool operator<(const SpdySessionKey& other) const;

  
  bool Equals(const SpdySessionKey& other) const;

  const HostPortProxyPair& host_port_proxy_pair() const {
    return host_port_proxy_pair_;
  }

  const HostPortPair& host_port_pair() const {
    return host_port_proxy_pair_.first;
  }

  const ProxyServer& proxy_server() const {
    return host_port_proxy_pair_.second;
  }

  PrivacyMode privacy_mode() const {
    return privacy_mode_;
  }

 private:
  HostPortProxyPair host_port_proxy_pair_;
  
  PrivacyMode privacy_mode_;
};

}  

#endif  

