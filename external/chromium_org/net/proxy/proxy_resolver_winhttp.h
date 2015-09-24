// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_RESOLVER_WINHTTP_H_
#define NET_PROXY_PROXY_RESOLVER_WINHTTP_H_

#include "base/compiler_specific.h"
#include "net/proxy/proxy_resolver.h"
#include "url/gurl.h"

typedef void* HINTERNET;  

namespace net {

class NET_EXPORT_PRIVATE ProxyResolverWinHttp : public ProxyResolver {
 public:
  ProxyResolverWinHttp();
  virtual ~ProxyResolverWinHttp();

  
  virtual int GetProxyForURL(const GURL& url,
                             ProxyInfo* results,
                             const net::CompletionCallback& ,
                             RequestHandle* ,
                             const BoundNetLog& ) OVERRIDE;
  virtual void CancelRequest(RequestHandle request) OVERRIDE;

  virtual LoadState GetLoadState(RequestHandle request) const OVERRIDE;

  virtual void CancelSetPacScript() OVERRIDE;

  virtual int SetPacScript(
      const scoped_refptr<ProxyResolverScriptData>& script_data,
      const net::CompletionCallback& ) OVERRIDE;

 private:
  bool OpenWinHttpSession();
  void CloseWinHttpSession();

  
  HINTERNET session_handle_;

  GURL pac_url_;

  DISALLOW_COPY_AND_ASSIGN(ProxyResolverWinHttp);
};

}  

#endif  
