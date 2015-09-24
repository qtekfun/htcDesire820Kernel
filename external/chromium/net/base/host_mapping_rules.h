// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_BASE_HOST_MAPPING_RULES_H_
#define NET_BASE_HOST_MAPPING_RULES_H_
#pragma once

#include <string>
#include <vector>
#include "base/basictypes.h"

namespace net {

class HostPortPair;

class HostMappingRules {
 public:
  HostMappingRules();
  ~HostMappingRules();

  
  
  bool RewriteHost(HostPortPair* host_port) const;

  
  
  
  
  
  
  
  
  bool AddRuleFromString(const std::string& rule_string);

  
  void SetRulesFromString(const std::string& rules_string);

 private:
  struct MapRule;
  struct ExclusionRule;

  typedef std::vector<MapRule> MapRuleList;
  typedef std::vector<ExclusionRule> ExclusionRuleList;

  MapRuleList map_rules_;
  ExclusionRuleList exclusion_rules_;

  DISALLOW_COPY_AND_ASSIGN(HostMappingRules);
};

}  

#endif  
