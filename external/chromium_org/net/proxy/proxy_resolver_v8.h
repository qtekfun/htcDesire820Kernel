// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_RESOLVER_V8_H_
#define NET_PROXY_PROXY_RESOLVER_V8_H_

#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/proxy/proxy_resolver.h"

namespace v8 {
class HeapStatistics;
class Isolate;
}  

namespace net {

class NET_EXPORT_PRIVATE ProxyResolverV8 : public ProxyResolver {
 public:
  
  class NET_EXPORT_PRIVATE JSBindings {
   public:
    enum ResolveDnsOperation {
      DNS_RESOLVE,
      DNS_RESOLVE_EX,
      MY_IP_ADDRESS,
      MY_IP_ADDRESS_EX,
    };

    JSBindings() {}

    
    
    
    
    virtual bool ResolveDns(const std::string& host,
                            ResolveDnsOperation op,
                            std::string* output,
                            bool* terminate) = 0;

    
    virtual void Alert(const base::string16& message) = 0;

    
    
    virtual void OnError(int line_number, const base::string16& error) = 0;

   protected:
    virtual ~JSBindings() {}
  };

  
  ProxyResolverV8();

  virtual ~ProxyResolverV8();

  JSBindings* js_bindings() const { return js_bindings_; }
  void set_js_bindings(JSBindings* js_bindings) { js_bindings_ = js_bindings; }

  
  virtual int GetProxyForURL(const GURL& url,
                             ProxyInfo* results,
                             const net::CompletionCallback& ,
                             RequestHandle* ,
                             const BoundNetLog& net_log) OVERRIDE;
  virtual void CancelRequest(RequestHandle request) OVERRIDE;
  virtual LoadState GetLoadState(RequestHandle request) const OVERRIDE;
  virtual void CancelSetPacScript() OVERRIDE;
  virtual void PurgeMemory() OVERRIDE;
  virtual int SetPacScript(
      const scoped_refptr<ProxyResolverScriptData>& script_data,
      const net::CompletionCallback& ) OVERRIDE;

  
  
  static void RememberDefaultIsolate();

#if defined(OS_WIN)
  
  
  
  
  
  static void CreateIsolate();
#endif

  static v8::Isolate* GetDefaultIsolate();

  
  
  static size_t GetTotalHeapSize();
  static size_t GetUsedHeapSize();

 private:
  static v8::Isolate* g_default_isolate_;

  
  
  
  class Context;

  scoped_ptr<Context> context_;

  JSBindings* js_bindings_;

  DISALLOW_COPY_AND_ASSIGN(ProxyResolverV8);
};

}  

#endif  
