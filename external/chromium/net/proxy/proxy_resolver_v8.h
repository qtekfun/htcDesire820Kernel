// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_RESOLVER_V8_H_
#define NET_PROXY_PROXY_RESOLVER_V8_H_
#pragma once

#include "base/memory/scoped_ptr.h"
#include "net/proxy/proxy_resolver.h"

namespace net {

class ProxyResolverJSBindings;

class ProxyResolverV8 : public ProxyResolver {
 public:
  
  
  
  explicit ProxyResolverV8(ProxyResolverJSBindings* custom_js_bindings);

  virtual ~ProxyResolverV8();

  ProxyResolverJSBindings* js_bindings() const { return js_bindings_.get(); }

  
  virtual int GetProxyForURL(const GURL& url,
                             ProxyInfo* results,
                             CompletionCallback* ,
                             RequestHandle* ,
                             const BoundNetLog& net_log);
  virtual void CancelRequest(RequestHandle request);
  virtual void CancelSetPacScript();
  virtual void PurgeMemory();
  virtual void Shutdown();
  virtual int SetPacScript(
      const scoped_refptr<ProxyResolverScriptData>& script_data,
      CompletionCallback* );

 private:
  
  
  
  class Context;
  scoped_ptr<Context> context_;

  scoped_ptr<ProxyResolverJSBindings> js_bindings_;

  DISALLOW_COPY_AND_ASSIGN(ProxyResolverV8);
};

}  

#endif  
