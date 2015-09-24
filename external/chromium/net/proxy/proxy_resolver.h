// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_RESOLVER_H_
#define NET_PROXY_PROXY_RESOLVER_H_
#pragma once

#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "googleurl/src/gurl.h"
#include "net/base/completion_callback.h"
#include "net/proxy/proxy_resolver_script_data.h"

namespace net {

class BoundNetLog;
class ProxyInfo;

class ProxyResolver {
 public:
  
  typedef void* RequestHandle;

  
  explicit ProxyResolver(bool expects_pac_bytes)
      : expects_pac_bytes_(expects_pac_bytes) {}

  virtual ~ProxyResolver() {}

  
  
  
  
  
  // |*request| is written to, and can be passed to CancelRequest().
  virtual int GetProxyForURL(const GURL& url,
                             ProxyInfo* results,
                             CompletionCallback* callback,
                             RequestHandle* request,
                             const BoundNetLog& net_log) = 0;

  
  virtual void CancelRequest(RequestHandle request) = 0;

  
  
  
  
  bool expects_pac_bytes() const { return expects_pac_bytes_; }

  virtual void CancelSetPacScript() = 0;

  
  
  
  virtual void PurgeMemory() {}

  
  
  
  virtual int SetPacScript(
      const scoped_refptr<ProxyResolverScriptData>& pac_script,
      CompletionCallback* callback) = 0;

  
  
  virtual void Shutdown() {}

 private:
  const bool expects_pac_bytes_;

  DISALLOW_COPY_AND_ASSIGN(ProxyResolver);
};

}  

#endif  
