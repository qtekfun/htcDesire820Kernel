// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_RESOLVER_REQUEST_CONTEXT_H_
#define NET_PROXY_PROXY_RESOLVER_REQUEST_CONTEXT_H_
#pragma once

namespace net {

class HostCache;
class BoundNetLog;

struct ProxyResolverRequestContext {
  
  
  ProxyResolverRequestContext(const BoundNetLog* net_log,
                              HostCache* host_cache)
    : net_log(net_log),
      host_cache(host_cache) {
  }

  const BoundNetLog* net_log;
  HostCache* host_cache;
};

}  

#endif  
