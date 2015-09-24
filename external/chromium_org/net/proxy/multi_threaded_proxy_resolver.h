// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_MULTI_THREADED_PROXY_RESOLVER_H_
#define NET_PROXY_MULTI_THREADED_PROXY_RESOLVER_H_

#include <deque>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/net_export.h"
#include "net/proxy/proxy_resolver.h"

namespace base {
class Thread;
}  

namespace net {

class ProxyResolverFactory {
 public:
  explicit ProxyResolverFactory(bool resolvers_expect_pac_bytes)
      : resolvers_expect_pac_bytes_(resolvers_expect_pac_bytes) {}

  virtual ~ProxyResolverFactory() {}

  
  
  virtual ProxyResolver* CreateProxyResolver() = 0;

  bool resolvers_expect_pac_bytes() const {
    return resolvers_expect_pac_bytes_;
  }

 private:
  bool resolvers_expect_pac_bytes_;
  DISALLOW_COPY_AND_ASSIGN(ProxyResolverFactory);
};

class NET_EXPORT_PRIVATE MultiThreadedProxyResolver
    : public ProxyResolver,
      NON_EXPORTED_BASE(public base::NonThreadSafe) {
 public:
  
  
  
  
  
  
  
  
  
  
  MultiThreadedProxyResolver(ProxyResolverFactory* resolver_factory,
                             size_t max_num_threads);

  virtual ~MultiThreadedProxyResolver();

  
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
  class Executor;
  class Job;
  class SetPacScriptJob;
  class GetProxyForURLJob;
  
  
  typedef std::deque<scoped_refptr<Job> > PendingJobsQueue;
  typedef std::vector<scoped_refptr<Executor> > ExecutorList;

  
  
  void CheckNoOutstandingUserRequests() const;

  
  void ReleaseAllExecutors();

  
  
  Executor* FindIdleExecutor();

  
  Executor* AddNewExecutor();

  
  void OnExecutorReady(Executor* executor);

  const scoped_ptr<ProxyResolverFactory> resolver_factory_;
  const size_t max_num_threads_;
  PendingJobsQueue pending_jobs_;
  ExecutorList executors_;
  scoped_refptr<ProxyResolverScriptData> current_script_data_;
};

}  

#endif  
