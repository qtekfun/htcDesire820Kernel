// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_HOST_RESOLVER_IMPL_H_
#define NET_DNS_HOST_RESOLVER_IMPL_H_

#include <map>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "net/base/capturing_net_log.h"
#include "net/base/net_export.h"
#include "net/base/network_change_notifier.h"
#include "net/base/prioritized_dispatcher.h"
#include "net/dns/host_cache.h"
#include "net/dns/host_resolver.h"
#include "net/dns/host_resolver_proc.h"

namespace net {

class BoundNetLog;
class DnsClient;
class NetLog;

class NET_EXPORT HostResolverImpl
    : public HostResolver,
      NON_EXPORTED_BASE(public base::NonThreadSafe),
      public NetworkChangeNotifier::IPAddressObserver,
      public NetworkChangeNotifier::DNSObserver {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  struct NET_EXPORT_PRIVATE ProcTaskParams {
    
    ProcTaskParams(HostResolverProc* resolver_proc, size_t max_retry_attempts);

    ~ProcTaskParams();

    
    
    scoped_refptr<HostResolverProc> resolver_proc;

    
    
    size_t max_retry_attempts;

    
    
    base::TimeDelta unresponsive_delay;

    
    uint32 retry_factor;
  };

  
  
  
  
  
  
  
  
  
  
  
  HostResolverImpl(scoped_ptr<HostCache> cache,
                   const PrioritizedDispatcher::Limits& job_limits,
                   const ProcTaskParams& proc_params,
                   NetLog* net_log);

  
  
  
  virtual ~HostResolverImpl();

  
  
  void SetMaxQueuedJobs(size_t value);

  
  
  
  
  void SetDnsClient(scoped_ptr<DnsClient> dns_client);

  
  virtual int Resolve(const RequestInfo& info,
                      RequestPriority priority,
                      AddressList* addresses,
                      const CompletionCallback& callback,
                      RequestHandle* out_req,
                      const BoundNetLog& source_net_log) OVERRIDE;
  virtual int ResolveFromCache(const RequestInfo& info,
                               AddressList* addresses,
                               const BoundNetLog& source_net_log) OVERRIDE;
  virtual void CancelRequest(RequestHandle req) OVERRIDE;
  virtual void SetDefaultAddressFamily(AddressFamily address_family) OVERRIDE;
  virtual AddressFamily GetDefaultAddressFamily() const OVERRIDE;
  virtual void SetDnsClientEnabled(bool enabled) OVERRIDE;
  virtual HostCache* GetHostCache() OVERRIDE;
  virtual base::Value* GetDnsConfigAsValue() const OVERRIDE;

 private:
  friend class HostResolverImplTest;
  class Job;
  class ProcTask;
  class LoopbackProbeJob;
  class DnsTask;
  class Request;
  typedef HostCache::Key Key;
  typedef std::map<Key, Job*> JobMap;
  typedef ScopedVector<Request> RequestsList;

  
  
  static const unsigned kMaximumDnsFailures;

  
  
  
  
  int ResolveHelper(const Key& key,
                    const RequestInfo& info,
                    AddressList* addresses,
                    const BoundNetLog& request_net_log);

  
  
  bool ResolveAsIP(const Key& key,
                   const RequestInfo& info,
                   int* net_error,
                   AddressList* addresses);

  
  
  
  bool ServeFromCache(const Key& key,
                      const RequestInfo& info,
                      int* net_error,
                      AddressList* addresses);

  
  
  bool ServeFromHosts(const Key& key,
                      const RequestInfo& info,
                      AddressList* addresses);

  
  void SetHaveOnlyLoopbackAddresses(bool result);

  
  
  
  Key GetEffectiveKeyForRequest(const RequestInfo& info,
                                const BoundNetLog& net_log) const;

  
  void CacheResult(const Key& key,
                   const HostCache::Entry& entry,
                   base::TimeDelta ttl);

  
  void RemoveJob(Job* job);

  
  
  void AbortAllInProgressJobs();

  
  
  
  void AbortDnsTasks();

  
  
  void TryServingAllJobsFromHosts();

  
  virtual void OnIPAddressChanged() OVERRIDE;

  
  virtual void OnDNSChanged() OVERRIDE;

  
  bool HaveDnsConfig() const;

  
  
  void OnDnsTaskResolve(int net_error);

  
  
  
  size_t num_running_dispatcher_jobs_for_tests() const {
    return dispatcher_.num_running_jobs();
  }

  
  scoped_ptr<HostCache> cache_;

  
  JobMap jobs_;

  
  PrioritizedDispatcher dispatcher_;

  
  size_t max_queued_jobs_;

  
  ProcTaskParams proc_params_;

  NetLog* net_log_;

  
  AddressFamily default_address_family_;

  base::WeakPtrFactory<HostResolverImpl> weak_ptr_factory_;

  base::WeakPtrFactory<HostResolverImpl> probe_weak_ptr_factory_;

  
  scoped_ptr<DnsClient> dns_client_;

  
  
  bool received_dns_config_;

  
  unsigned num_dns_failures_;

  
  
  bool probe_ipv6_support_;

  
  
  bool use_local_ipv6_;

  
  
  bool resolved_known_ipv6_hostname_;

  
  HostResolverFlags additional_resolver_flags_;

  
  bool fallback_to_proctask_;

  DISALLOW_COPY_AND_ASSIGN(HostResolverImpl);
};

}  

#endif  
