// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_LIST_H_
#define NET_PROXY_PROXY_LIST_H_
#pragma once

#include <string>
#include <vector>

#include "net/proxy/proxy_retry_info.h"

namespace net {

class ProxyServer;

class ProxyList {
 public:
  ProxyList();
  ~ProxyList();

  
  
  void Set(const std::string& proxy_uri_list);

  
  void SetSingleProxyServer(const ProxyServer& proxy_server);

  
  
  void DeprioritizeBadProxies(const ProxyRetryInfoMap& proxy_retry_info);

  
  
  void RemoveProxiesWithoutScheme(int scheme_bit_field);

  
  bool IsEmpty() const;

  
  
  const ProxyServer& Get() const;

  
  
  
  
  
  
  void SetFromPacString(const std::string& pac_string);

  
  
  std::string ToPacString() const;

  
  
  
  bool Fallback(ProxyRetryInfoMap* proxy_retry_info);

 private:
  
  std::vector<ProxyServer> proxies_;
};

}  

#endif  
