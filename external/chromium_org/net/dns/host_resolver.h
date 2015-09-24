// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_HOST_RESOLVER_H_
#define NET_DNS_HOST_RESOLVER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "net/base/address_family.h"
#include "net/base/completion_callback.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_export.h"
#include "net/base/net_util.h"
#include "net/base/request_priority.h"

namespace base {
class Value;
}

namespace net {

class AddressList;
class BoundNetLog;
class HostCache;
class HostResolverProc;
class NetLog;

class NET_EXPORT HostResolver {
 public:
  
  
  
  
  
  
  
  struct NET_EXPORT Options {
    Options();

    size_t max_concurrent_resolves;
    size_t max_retry_attempts;
    bool enable_caching;
  };

  
  
  class NET_EXPORT RequestInfo {
   public:
    explicit RequestInfo(const HostPortPair& host_port_pair);

    const HostPortPair& host_port_pair() const { return host_port_pair_; }
    void set_host_port_pair(const HostPortPair& host_port_pair) {
      host_port_pair_ = host_port_pair;
    }

    int port() const { return host_port_pair_.port(); }
    const std::string& hostname() const { return host_port_pair_.host(); }

    AddressFamily address_family() const { return address_family_; }
    void set_address_family(AddressFamily address_family) {
      address_family_ = address_family;
    }

    HostResolverFlags host_resolver_flags() const {
      return host_resolver_flags_;
    }
    void set_host_resolver_flags(HostResolverFlags host_resolver_flags) {
      host_resolver_flags_ = host_resolver_flags;
    }

    bool allow_cached_response() const { return allow_cached_response_; }
    void set_allow_cached_response(bool b) { allow_cached_response_ = b; }

    bool is_speculative() const { return is_speculative_; }
    void set_is_speculative(bool b) { is_speculative_ = b; }

   private:
    
    HostPortPair host_port_pair_;

    
    AddressFamily address_family_;

    
    HostResolverFlags host_resolver_flags_;

    
    bool allow_cached_response_;

    
    bool is_speculative_;
  };

  
  typedef void* RequestHandle;

  
  
  
  static const size_t kDefaultParallelism = 0;

  
  
  static const size_t kDefaultRetryAttempts = -1;

  
  
  
  virtual ~HostResolver();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Resolve(const RequestInfo& info,
                      RequestPriority priority,
                      AddressList* addresses,
                      const CompletionCallback& callback,
                      RequestHandle* out_req,
                      const BoundNetLog& net_log) = 0;

  
  
  
  
  virtual int ResolveFromCache(const RequestInfo& info,
                               AddressList* addresses,
                               const BoundNetLog& net_log) = 0;

  
  
  
  
  virtual void CancelRequest(RequestHandle req) = 0;

  
  
  
  
  virtual void SetDefaultAddressFamily(AddressFamily address_family) {}
  virtual AddressFamily GetDefaultAddressFamily() const;

  
  virtual void SetDnsClientEnabled(bool enabled);

  
  
  virtual HostCache* GetHostCache();

  
  
  
  virtual base::Value* GetDnsConfigAsValue() const;

  
  
  
  static scoped_ptr<HostResolver> CreateSystemResolver(
      const Options& options,
      NetLog* net_log);

  
  static scoped_ptr<HostResolver> CreateDefaultResolver(NetLog* net_log);

 protected:
  HostResolver();

 private:
  DISALLOW_COPY_AND_ASSIGN(HostResolver);
};

}  

#endif  
