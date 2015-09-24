// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_MAPPED_HOST_RESOLVER_H_
#define NET_BASE_MAPPED_HOST_RESOLVER_H_
#pragma once

#include <string>

#include "base/memory/scoped_ptr.h"
#include "net/base/host_mapping_rules.h"
#include "net/base/host_resolver.h"

namespace net {

class MappedHostResolver : public HostResolver {
 public:
  
  
  explicit MappedHostResolver(HostResolver* impl);
  virtual ~MappedHostResolver();

  
  
  
  
  
  
  
  
  bool AddRuleFromString(const std::string& rule_string) {
    return rules_.AddRuleFromString(rule_string);
  }

  
  void SetRulesFromString(const std::string& rules_string) {
    rules_.SetRulesFromString(rules_string);
  }

  
  virtual int Resolve(const RequestInfo& info,
                      AddressList* addresses,
                      CompletionCallback* callback,
                      RequestHandle* out_req,
                      const BoundNetLog& net_log);
  virtual void CancelRequest(RequestHandle req);
  virtual void AddObserver(Observer* observer);
  virtual void RemoveObserver(Observer* observer);
  virtual HostResolverImpl* GetAsHostResolverImpl();

 private:
  scoped_ptr<HostResolver> impl_;

  HostMappingRules rules_;
};

}  

#endif  
