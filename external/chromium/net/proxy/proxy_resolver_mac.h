// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_RESOLVER_MAC_H_
#define NET_PROXY_PROXY_RESOLVER_MAC_H_
#pragma once

#include "googleurl/src/gurl.h"
#include "net/base/net_errors.h"
#include "net/proxy/proxy_resolver.h"

namespace net {

class ProxyResolverMac : public ProxyResolver {
 public:
  ProxyResolverMac();
  virtual ~ProxyResolverMac();

  
  virtual int GetProxyForURL(const GURL& url,
                             ProxyInfo* results,
                             CompletionCallback* callback,
                             RequestHandle* request,
                             const BoundNetLog& net_log);

  virtual void CancelRequest(RequestHandle request);

  virtual void CancelSetPacScript();

  virtual int SetPacScript(
      const scoped_refptr<ProxyResolverScriptData>& script_data,
      CompletionCallback* );

 private:
  scoped_refptr<ProxyResolverScriptData> script_data_;
};

}  

#endif  
