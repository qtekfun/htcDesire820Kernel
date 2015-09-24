// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_RESOLVER_JS_BINDINGS_H_
#define NET_PROXY_PROXY_RESOLVER_JS_BINDINGS_H_
#pragma once

#include <string>

#include "base/string16.h"

namespace net {

class HostResolver;
class NetLog;
struct ProxyResolverRequestContext;

class ProxyResolverJSBindings {
 public:
  ProxyResolverJSBindings() : current_request_context_(NULL) {}

  virtual ~ProxyResolverJSBindings() {}

  
  virtual void Alert(const string16& message) = 0;

  
  
  virtual bool MyIpAddress(std::string* first_ip_address) = 0;

  
  
  
  
  

  virtual bool MyIpAddressEx(std::string* ip_address_list) = 0;

  
  
  virtual bool DnsResolve(const std::string& host,
                          std::string* first_ip_address) = 0;

  
  
  
  
  
  virtual bool DnsResolveEx(const std::string& host,
                            std::string* ip_address_list) = 0;

  
  
  virtual void OnError(int line_number, const string16& error) = 0;

  
  virtual void Shutdown() = 0;

  
  
  
  
  
  
  static ProxyResolverJSBindings* CreateDefault(HostResolver* host_resolver,
                                                NetLog* net_log);

  
  void set_current_request_context(
      ProxyResolverRequestContext* current_request_context) {
    current_request_context_ = current_request_context;
  }

  
  ProxyResolverRequestContext* current_request_context() {
    return current_request_context_;
  }

 private:
  ProxyResolverRequestContext* current_request_context_;
};

}  

#endif  
