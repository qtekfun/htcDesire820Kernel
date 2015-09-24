// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_RESOLVER_H_
#define NET_PROXY_PROXY_RESOLVER_H_

#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string16.h"
#include "net/base/completion_callback.h"
#include "net/base/load_states.h"
#include "net/base/net_export.h"
#include "net/proxy/proxy_resolver_script_data.h"
#include "url/gurl.h"

namespace net {

class BoundNetLog;
class ProxyInfo;

class NET_EXPORT_PRIVATE ProxyResolver {
 public:
  
  typedef void* RequestHandle;

  
  explicit ProxyResolver(bool expects_pac_bytes)
      : expects_pac_bytes_(expects_pac_bytes) {}

  virtual ~ProxyResolver() {}

  
  
  
  
  
  // |*request| is written to, and can be passed to CancelRequest().
  virtual int GetProxyForURL(const GURL& url,
                             ProxyInfo* results,
                             const net::CompletionCallback& callback,
                             RequestHandle* request,
                             const BoundNetLog& net_log) = 0;

  
  virtual void CancelRequest(RequestHandle request) = 0;

  
  virtual LoadState GetLoadState(RequestHandle request) const = 0;

  
  
  
  
  bool expects_pac_bytes() const { return expects_pac_bytes_; }

  virtual void CancelSetPacScript() = 0;

  
  
  
  virtual void PurgeMemory() {}

  
  
  
  virtual int SetPacScript(
      const scoped_refptr<ProxyResolverScriptData>& pac_script,
      const net::CompletionCallback& callback) = 0;

 private:
  const bool expects_pac_bytes_;

  DISALLOW_COPY_AND_ASSIGN(ProxyResolver);
};

}  

#endif  
