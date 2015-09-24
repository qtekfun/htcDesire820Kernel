// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_MOCK_HOST_RESOLVER_H_
#define NET_BASE_MOCK_HOST_RESOLVER_H_
#pragma once

#include <list>

#include "base/synchronization/waitable_event.h"
#include "net/base/host_resolver_impl.h"
#include "net/base/host_resolver_proc.h"

namespace net {

class RuleBasedHostResolverProc;


class MockHostResolverBase : public HostResolver {
 public:
  virtual ~MockHostResolverBase();

  RuleBasedHostResolverProc* rules() { return rules_; }

  
  void set_synchronous_mode(bool is_synchronous) {
    synchronous_mode_ = is_synchronous;
  }

  
  void Reset(HostResolverProc* interceptor);

  void SetPoolConstraints(HostResolverImpl::JobPoolIndex pool_index,
                          size_t max_outstanding_jobs,
                          size_t max_pending_requests) {
    impl_->SetPoolConstraints(
        pool_index, max_outstanding_jobs, max_pending_requests);
  }

  
  virtual int Resolve(const RequestInfo& info,
                      AddressList* addresses,
                      CompletionCallback* callback,
                      RequestHandle* out_req,
                      const BoundNetLog& net_log);
  virtual void CancelRequest(RequestHandle req);
  virtual void AddObserver(Observer* observer);
  virtual void RemoveObserver(Observer* observer);

 protected:
  MockHostResolverBase(bool use_caching);

  scoped_ptr<HostResolverImpl> impl_;
  scoped_refptr<RuleBasedHostResolverProc> rules_;
  bool synchronous_mode_;
  bool use_caching_;

 private:
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
  ~MockCachingHostResolver() {}
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

  
  virtual int Resolve(const std::string& host,
                      AddressFamily address_family,
                      HostResolverFlags host_resolver_flags,
                      AddressList* addrlist,
                      int* os_error);

 private:
  struct Rule;
  typedef std::list<Rule> RuleList;

  ~RuleBasedHostResolverProc();

  RuleList rules_;
};

class WaitingHostResolverProc : public HostResolverProc {
 public:
  explicit WaitingHostResolverProc(HostResolverProc* previous);

  void Signal();

  
  virtual int Resolve(const std::string& host,
                      AddressFamily address_family,
                      HostResolverFlags host_resolver_flags,
                      AddressList* addrlist,
                      int* os_error);

 private:
  virtual ~WaitingHostResolverProc();

  base::WaitableEvent event_;
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
