// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Copyright (c) 2011, Code Aurora Forum. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_HOST_RESOLVER_H_
#define NET_BASE_HOST_RESOLVER_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "googleurl/src/gurl.h"
#include "net/base/address_family.h"
#include "net/base/completion_callback.h"
#include "net/base/host_port_pair.h"
#include "net/base/net_export.h"
#include "net/base/request_priority.h"

class MessageLoop;

namespace net {

class AddressList;
class BoundNetLog;
class HostResolverImpl;
class HostResolverProc;
class NetLog;

class NET_EXPORT HostResolver {
 public:
  
  
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

    bool only_use_cached_response() const { return only_use_cached_response_; }
    void set_only_use_cached_response(bool b) { only_use_cached_response_ = b; }

    bool is_speculative() const { return is_speculative_; }
    void set_is_speculative(bool b) { is_speculative_ = b; }

    RequestPriority priority() const { return priority_; }
    void set_priority(RequestPriority priority) { priority_ = priority; }

    const GURL& referrer() const { return referrer_; }
    void set_referrer(const GURL& referrer) { referrer_ = referrer; }

   private:
    
    HostPortPair host_port_pair_;

    
    AddressFamily address_family_;

    
    HostResolverFlags host_resolver_flags_;

    
    bool allow_cached_response_;

    
    bool only_use_cached_response_;

    
    bool is_speculative_;

    
    RequestPriority priority_;

    
    
    GURL referrer_;
  };

  
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    
    virtual void OnStartResolution(int id, const RequestInfo& info) = 0;

    
    
    virtual void OnFinishResolutionWithStatus(int id, bool was_resolved,
                                              const RequestInfo& info) = 0;

    
    
    
    virtual void OnCancelResolution(int id, const RequestInfo& info) = 0;
  };

  class HostnameResolverExt {
  public:
      virtual ~HostnameResolverExt() {}
      virtual void Resolve()=0;
  };

  virtual void SetResolverExt(HostnameResolverExt* preresolver) {};
  
  typedef void* RequestHandle;

  
  
  
  static const size_t kDefaultParallelism = 0;

  
  
  
  virtual ~HostResolver();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int Resolve(const RequestInfo& info,
                      AddressList* addresses,
                      CompletionCallback* callback,
                      RequestHandle* out_req,
                      const BoundNetLog& net_log) = 0;

  
  
  virtual void CancelRequest(RequestHandle req) = 0;

  
  
  
  virtual void AddObserver(Observer* observer) = 0;

  
  virtual void RemoveObserver(Observer* observer) = 0;

  
  
  
  
  virtual void SetDefaultAddressFamily(AddressFamily address_family) {}
  virtual AddressFamily GetDefaultAddressFamily() const;

  
  
  
  virtual HostResolverImpl* GetAsHostResolverImpl();

  
  virtual void Shutdown() {}

 protected:
  HostResolver();

 private:
  DISALLOW_COPY_AND_ASSIGN(HostResolver);
};

class NET_EXPORT SingleRequestHostResolver {
 public:
  
  explicit SingleRequestHostResolver(HostResolver* resolver);

  
  
  
  ~SingleRequestHostResolver();

  
  
  int Resolve(const HostResolver::RequestInfo& info,
              AddressList* addresses,
              CompletionCallback* callback,
              const BoundNetLog& net_log);

  
  
  void Cancel();

 private:
  
  
  void OnResolveCompletion(int result);

  
  HostResolver* const resolver_;

  
  HostResolver::RequestHandle cur_request_;
  CompletionCallback* cur_request_callback_;

  
  CompletionCallbackImpl<SingleRequestHostResolver> callback_;

  DISALLOW_COPY_AND_ASSIGN(SingleRequestHostResolver);
};

NET_EXPORT HostResolver* CreateSystemHostResolver(size_t max_concurrent_resolves,
                                                  HostResolverProc* resolver_proc,
                                                  NetLog* net_log);

HostResolver* CreateSystemHostResolver(size_t max_concurrent_resolves,
                                       HostResolverProc* resolver_proc,
                                       NetLog* net_log,
                                       MessageLoop* net_notification_messageloop);
}  

#endif  
