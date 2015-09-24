// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Copyright (c) 2011, Code Aurora Forum. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_HOST_RESOLVER_IMPL_H_
#define NET_BASE_HOST_RESOLVER_IMPL_H_
#pragma once

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "net/base/capturing_net_log.h"
#include "net/base/host_cache.h"
#include "net/base/host_resolver.h"
#include "net/base/host_resolver_proc.h"
#include "net/base/net_log.h"
#include "net/base/network_change_notifier.h"

namespace net {


class HostResolverImpl : public HostResolver,
                         public base::NonThreadSafe,
                         public NetworkChangeNotifier::IPAddressObserver {
 public:
  
  
  
  
  
  enum JobPoolIndex {
    POOL_NORMAL = 0,
    POOL_COUNT,
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  HostResolverImpl(HostResolverProc* resolver_proc,
                   HostCache* cache,
                   size_t max_jobs,
                   NetLog* net_log,
                   MessageLoop* net_notification_messageloop=NULL
                   );

  
  
  
  virtual ~HostResolverImpl();

  
  
  void ProbeIPv6Support();

  
  HostCache* cache() { return cache_.get(); }

  
  
  
  
  
  
  
  
  
  
  
  void SetPoolConstraints(JobPoolIndex pool_index,
                          size_t max_outstanding_jobs,
                          size_t max_pending_requests);

  
  virtual int Resolve(const RequestInfo& info,
                      AddressList* addresses,
                      CompletionCallback* callback,
                      RequestHandle* out_req,
                      const BoundNetLog& source_net_log);
  virtual void CancelRequest(RequestHandle req);
  virtual void AddObserver(HostResolver::Observer* observer);
  virtual void RemoveObserver(HostResolver::Observer* observer);

  
  virtual void SetDefaultAddressFamily(AddressFamily address_family);
  virtual AddressFamily GetDefaultAddressFamily() const;

  virtual HostResolverImpl* GetAsHostResolverImpl();

  
  virtual void Shutdown();

  virtual void SetResolverExt(HostnameResolverExt* preresolver);
 private:
  class Job;
  class JobPool;
  class IPv6ProbeJob;
  class Request;
  typedef std::vector<Request*> RequestsList;
  typedef HostCache::Key Key;
  typedef std::map<Key, scoped_refptr<Job> > JobMap;
  typedef std::vector<HostResolver::Observer*> ObserversList;

  
  HostResolverProc* effective_resolver_proc() const {
    return resolver_proc_ ?
        resolver_proc_.get() : HostResolverProc::GetDefault();
  }

  
  void AddOutstandingJob(Job* job);

  
  Job* FindOutstandingJob(const Key& key);

  
  void RemoveOutstandingJob(Job* job);

  
  void OnJobComplete(Job* job, int net_error, int os_error,
                     const AddressList& addrlist);

  
  
  void AbortJob(Job* job);

  
  void OnJobCompleteInternal(Job* job, int net_error, int os_error,
                             const AddressList& addrlist);

  
  void OnStartRequest(const BoundNetLog& source_net_log,
                      const BoundNetLog& request_net_log,
                      int request_id,
                      const RequestInfo& info);

  
  void OnFinishRequest(const BoundNetLog& source_net_log,
                       const BoundNetLog& request_net_log,
                       int request_id,
                       const RequestInfo& info,
                       int net_error,
                       int os_error);

  
  void OnCancelRequest(const BoundNetLog& source_net_log,
                       const BoundNetLog& request_net_log,
                       int request_id,
                       const RequestInfo& info);

  
  void DiscardIPv6ProbeJob();

  
  void IPv6ProbeSetDefaultAddressFamily(AddressFamily address_family);

  
  
  bool CanCreateJobForPool(const JobPool& pool) const;

  
  static JobPoolIndex GetJobPoolIndexForRequest(const Request* req);

  JobPool* GetPoolForRequest(const Request* req) {
    return job_pools_[GetJobPoolIndexForRequest(req)];
  }

  
  
  void ProcessQueuedRequests();

  
  
  
  Key GetEffectiveKeyForRequest(const RequestInfo& info) const;

  
  Job* CreateAndStartJob(Request* req);

  
  int EnqueueRequest(JobPool* pool, Request* req);

  
  void CancelAllJobs();

  
  void AbortAllInProgressJobs();

  
  virtual void OnIPAddressChanged();

  
  scoped_ptr<HostCache> cache_;

  
  JobMap jobs_;

  
  size_t max_jobs_;

  
  
  JobPool* job_pools_[POOL_COUNT];

  
  
  scoped_refptr<Job> cur_completing_job_;

  
  ObserversList observers_;

  
  
  int next_request_id_;

  
  
  int next_job_id_;

  
  
  scoped_refptr<HostResolverProc> resolver_proc_;

  
  AddressFamily default_address_family_;

  
  bool shutdown_;

  
  
  
  bool ipv6_probe_monitoring_;

  
  scoped_refptr<IPv6ProbeJob> ipv6_probe_job_;

  
  HostResolverFlags additional_resolver_flags_;

  NetLog* net_log_;

  MessageLoop* net_notification_messageloop_;
  HostnameResolverExt* resolverext_;

  DISALLOW_COPY_AND_ASSIGN(HostResolverImpl);
};

}  

#endif  
