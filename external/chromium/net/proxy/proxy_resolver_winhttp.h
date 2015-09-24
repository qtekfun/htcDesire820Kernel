// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_RESOLVER_WINHTTP_H_
#define NET_PROXY_PROXY_RESOLVER_WINHTTP_H_
#pragma once

#include "googleurl/src/gurl.h"
#include "net/proxy/proxy_resolver.h"

typedef void* HINTERNET;  

namespace net {

class ProxyResolverWinHttp : public ProxyResolver {
 public:
  ProxyResolverWinHttp();
  virtual ~ProxyResolverWinHttp();

  
  virtual int GetProxyForURL(const GURL& url,
                             ProxyInfo* results,
                             CompletionCallback* ,
                             RequestHandle* ,
                             const BoundNetLog& );
  virtual void CancelRequest(RequestHandle request);

  virtual void CancelSetPacScript();

  virtual int SetPacScript(
      const scoped_refptr<ProxyResolverScriptData>& script_data,
      CompletionCallback* );

 private:
  bool OpenWinHttpSession();
  void CloseWinHttpSession();

  
  HINTERNET session_handle_;

  GURL pac_url_;

  DISALLOW_COPY_AND_ASSIGN(ProxyResolverWinHttp);
};

}  

#endif  
