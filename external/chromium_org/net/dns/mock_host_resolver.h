// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_MOCK_HOST_RESOLVER_H_
#define NET_DNS_MOCK_HOST_RESOLVER_H_

#include <list>
#include <map>

#include "base/memory/weak_ptr.h"
#include "base/synchronization/waitable_event.h"
#include "base/threading/non_thread_safe.h"
#include "net/dns/host_resolver.h"
#include "net/dns/host_resolver_proc.h"

namespace net {

class HostCache;
class RuleBasedHostResolverProc;

int ParseAddressList(const std::string& host_list,
                     const std::string& canonical_name,
                     AddressList* addrlist);


class MockHostResolverBase : public HostResolver,
                             public base::SupportsWeakPtr<MockHostResolverBase>,
                             public base::NonThreadSafe {
 public:
  virtual ~MockHostResolverBase();

  RuleBasedHostResolverProc* rules() { return rules_.get(); }
  void set_rules(RuleBasedHostResolverProc* rules) { rules_ = rules; }

  
  void set_synchronous_mode(bool is_synchronous) {
    synchronous_mode_ = is_synchronous;
  }

  
  
  
  
  void set_ondemand_mode(bool is_ondemand) {
    ondemand_mode_ = is_ondemand;
  }

  
  virtual int Resolve(const RequestInfo& info,
                      RequestPriority priority,
                      AddressList* addresses,
                      const CompletionCallback& callback,
                      RequestHandle* out_req,
                      const BoundNetLog& net_log) OVERRIDE;
  virtual int ResolveFromCache(const RequestInfo& info,
                               AddressList* addresses,
                               const BoundNetLog& net_log) OVERRIDE;
  virtual void CancelRequest(RequestHandle req) OVERRIDE;
  virtual HostCache* GetHostCache() OVERRIDE;

  
  
  
  void ResolveAllPending();

  
  
  bool has_pending_requests() const { return !requests_.empty(); }

  
  size_t num_resolve() const {
    return num_resolve_;
  }

  
  size_t num_resolve_from_cache() const {
    return num_resolve_from_cache_;
  }

  
  
  RequestPriority last_request_priority() const {
    return last_request_priority_;
  }

 protected:
  explicit MockHostResolverBase(bool use_caching);

 private:
  struct Request;
  typedef std::map<size_t, Request*> RequestMap;

  
  
  int ResolveFromIPLiteralOrCache(const RequestInfo& info,
                                  AddressList* addresses);
  
  int ResolveProc(size_t id, const RequestInfo& info, AddressList* addresses);
  
  void ResolveNow(size_t id);

  RequestPriority last_request_priority_;
  bool synchronous_mode_;
  bool ondemand_mode_;
  scoped_refptr<RuleBasedHostResolverProc> rules_;
  scoped_ptr<HostCache> cache_;
  RequestMap requests_;
  size_t next_request_id_;

  size_t num_resolve_;
  size_t num_resolve_from_cache_;

  DISALLOW_COPY_AND_ASSIGN(MockHostResolverBase);
};

class MockHostResolver : public MockHostResolverBase {
 public:
  MockHostResolver() : MockHostResolverBase(false ) {}
  virtual ~MockHostResolver() {}
};

class MockCachingHostResolver : public MockHostResolverBase {
 public:
  MockCachingHostResolver() : MockHostResolverBase(true ) {}
  virtual ~MockCachingHostResolver() {}
};

class RuleBasedHostResolverProc : public HostResolverProc {
 public:
  explicit RuleBasedHostResolverProc(HostResolverProc* previous);

  
  
  void AddRule(const std::string& host_pattern,
               const std::string& replacement);

  
  void AddRuleForAddressFamily(const std::string& host_pattern,
                               AddressFamily address_family,
                               const std::string& replacement);

  
  
  
  
  
  
  
  void AddIPLiteralRule(const std::string& host_pattern,
                        const std::string& ip_literal,
                        const std::string& canonical_name);

  void AddRuleWithLatency(const std::string& host_pattern,
                          const std::string& replacement,
                          int latency_ms);

  
  
  void AllowDirectLookup(const std::string& host);

  
  void AddSimulatedFailure(const std::string& host);

  
  void ClearRules();

  
  virtual int Resolve(const std::string& host,
                      AddressFamily address_family,
                      HostResolverFlags host_resolver_flags,
                      AddressList* addrlist,
                      int* os_error) OVERRIDE;

 private:
  struct Rule;
  typedef std::list<Rule> RuleList;

  virtual ~RuleBasedHostResolverProc();

  RuleList rules_;
};

RuleBasedHostResolverProc* CreateCatchAllHostResolverProc();

class HangingHostResolver : public HostResolver {
 public:
  virtual int Resolve(const RequestInfo& info,
                      RequestPriority priority,
                      AddressList* addresses,
                      const CompletionCallback& callback,
                      RequestHandle* out_req,
                      const BoundNetLog& net_log) OVERRIDE;
  virtual int ResolveFromCache(const RequestInfo& info,
                               AddressList* addresses,
                               const BoundNetLog& net_log) OVERRIDE;
  virtual void CancelRequest(RequestHandle req) OVERRIDE {}
};

class ScopedDefaultHostResolverProc {
 public:
  ScopedDefaultHostResolverProc();
  explicit ScopedDefaultHostResolverProc(HostResolverProc* proc);

  ~ScopedDefaultHostResolverProc();

  void Init(HostResolverProc* proc);

 private:
  scoped_refptr<HostResolverProc> current_proc_;
  scoped_refptr<HostResolverProc> previous_proc_;
};

}  

#endif  
