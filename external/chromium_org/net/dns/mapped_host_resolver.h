// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_MAPPED_HOST_RESOLVER_H_
#define NET_DNS_MAPPED_HOST_RESOLVER_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "net/base/host_mapping_rules.h"
#include "net/base/net_export.h"
#include "net/dns/host_resolver.h"

namespace net {

class NET_EXPORT MappedHostResolver : public HostResolver {
 public:
  
  
  explicit MappedHostResolver(scoped_ptr<HostResolver> impl);
  virtual ~MappedHostResolver();

  
  
  
  
  
  
  
  
  
  
  bool AddRuleFromString(const std::string& rule_string) {
    return rules_.AddRuleFromString(rule_string);
  }

  
  void SetRulesFromString(const std::string& rules_string) {
    rules_.SetRulesFromString(rules_string);
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
  virtual void SetDnsClientEnabled(bool enabled) OVERRIDE;
  virtual HostCache* GetHostCache() OVERRIDE;
  virtual base::Value* GetDnsConfigAsValue() const OVERRIDE;

 private:
  
  
  int ApplyRules(RequestInfo* info) const;

  scoped_ptr<HostResolver> impl_;

  HostMappingRules rules_;
};

}  

#endif  
