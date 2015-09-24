// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_RESOLVER_V8_TRACING_H_
#define NET_PROXY_PROXY_RESOLVER_V8_TRACING_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/net_export.h"
#include "net/proxy/proxy_resolver.h"

namespace base {
class Thread;
class MessageLoopProxy;
}  

namespace net {

class HostResolver;
class NetLog;
class ProxyResolverErrorObserver;
class ProxyResolverV8;

class NET_EXPORT_PRIVATE ProxyResolverV8Tracing
    : public ProxyResolver,
      NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  
  
  
  
  
  
  ProxyResolverV8Tracing(HostResolver* host_resolver,
                         ProxyResolverErrorObserver* error_observer,
                         NetLog* net_log);

  virtual ~ProxyResolverV8Tracing();

  
  virtual int GetProxyForURL(const GURL& url,
                             ProxyInfo* results,
                             const CompletionCallback& callback,
                             RequestHandle* request,
                             const BoundNetLog& net_log) OVERRIDE;
  virtual void CancelRequest(RequestHandle request) OVERRIDE;
  virtual LoadState GetLoadState(RequestHandle request) const OVERRIDE;
  virtual void CancelSetPacScript() OVERRIDE;
  virtual void PurgeMemory() OVERRIDE;
  virtual int SetPacScript(
      const scoped_refptr<ProxyResolverScriptData>& script_data,
      const CompletionCallback& callback) OVERRIDE;

 private:
  class Job;

  
  scoped_ptr<base::Thread> thread_;
  scoped_ptr<ProxyResolverV8> v8_resolver_;

  
  HostResolver* host_resolver_;

  scoped_ptr<ProxyResolverErrorObserver> error_observer_;
  NetLog* net_log_;

  
  scoped_refptr<Job> set_pac_script_job_;

  
  int num_outstanding_callbacks_;

  DISALLOW_COPY_AND_ASSIGN(ProxyResolverV8Tracing);
};

}  

#endif  
