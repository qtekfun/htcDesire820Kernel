// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_DNSRR_RESOLVER_H_
#define NET_BASE_DNSRR_RESOLVER_H_
#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/threading/non_thread_safe.h"
#include "base/time.h"
#include "build/build_config.h"
#include "net/base/completion_callback.h"
#include "net/base/network_change_notifier.h"

namespace net {

struct RRResponse {
  RRResponse();
  ~RRResponse();

  
  
  bool HasExpired(base::Time current_time) const;

  
  bool ParseFromResponse(const uint8* data, unsigned len,
                         uint16 rrtype_requested);

  
  
  std::string name;
  
  uint32 ttl;
  
  bool dnssec;
  std::vector<std::string> rrdatas;
  
  std::vector<std::string> signatures;
  
  
  base::Time fetch_time;
  
  
  bool negative;
};

class BoundNetLog;
class RRResolverWorker;
class RRResolverJob;

class DnsRRResolver : public base::NonThreadSafe,
                      public NetworkChangeNotifier::IPAddressObserver {
 public:
  typedef intptr_t Handle;

  enum {
    kInvalidHandle = 0,
  };

  enum {
    
    
    FLAG_WANT_DNSSEC = 1,
  };

  DnsRRResolver();
  ~DnsRRResolver();

  uint64 requests() const { return requests_; }
  uint64 cache_hits() const { return cache_hits_; }
  uint64 inflight_joins() const { return inflight_joins_; }

  
  
  
  
  
  
  
  
  Handle Resolve(const std::string& name, uint16 rrtype,
                 uint16 flags, CompletionCallback* callback,
                 RRResponse* response, int priority,
                 const BoundNetLog& netlog);

  
  
  void CancelResolve(Handle handle);

  
  virtual void OnIPAddressChanged();

 private:
  friend class RRResolverWorker;

  void HandleResult(const std::string& name, uint16 rrtype, int result,
                    const RRResponse& response);

  
  
  
  std::map<std::pair<std::string, uint16>, RRResponse> cache_;
  
  
  std::map<std::pair<std::string, uint16>, RRResolverJob*> inflight_;

  uint64 requests_;
  uint64 cache_hits_;
  uint64 inflight_joins_;

  bool in_destructor_;

  DISALLOW_COPY_AND_ASSIGN(DnsRRResolver);
};

}  

#endif  
