// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_PROXY_PROXY_BYPASS_RULES_H_
#define NET_PROXY_PROXY_BYPASS_RULES_H_

#include <string>
#include <vector>

#include "net/base/net_export.h"
#include "url/gurl.h"

namespace net {

class NET_EXPORT ProxyBypassRules {
 public:
  
  class NET_EXPORT Rule {
   public:
    Rule();
    virtual ~Rule();

    
    virtual bool Matches(const GURL& url) const = 0;

    
    
    virtual std::string ToString() const = 0;

    
    virtual Rule* Clone() const = 0;

    bool Equals(const Rule& rule) const;

   private:
    DISALLOW_COPY_AND_ASSIGN(Rule);
  };

  typedef std::vector<const Rule*> RuleList;

  
  ProxyBypassRules();
  ProxyBypassRules(const ProxyBypassRules& rhs);
  ~ProxyBypassRules();
  ProxyBypassRules& operator=(const ProxyBypassRules& rhs);

  
  
  
  const RuleList& rules() const { return rules_; }

  
  bool Matches(const GURL& url) const;

  
  
  bool Equals(const ProxyBypassRules& other) const;

  
  
  
  void ParseFromString(const std::string& raw);

  
  
  
  
  
  
  void ParseFromStringUsingSuffixMatching(const std::string& raw);

  
  
  
  
  
  
  
  bool AddRuleForHostname(const std::string& optional_scheme,
                          const std::string& hostname_pattern,
                          int optional_port);

  
  
  
  
  
  void AddRuleToBypassLocal();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool AddRuleFromString(const std::string& raw);

  
  
  
  
  
  
  
  
  
  bool AddRuleFromStringUsingSuffixMatching(const std::string& raw);

  
  
  std::string ToString() const;

  
  void Clear();

  
  void AssignFrom(const ProxyBypassRules& other);

 private:
  
  
  
  void ParseFromStringInternal(const std::string& raw,
                               bool use_hostname_suffix_matching);
  bool AddRuleFromStringInternal(const std::string& raw,
                                 bool use_hostname_suffix_matching);
  bool AddRuleFromStringInternalWithLogging(const std::string& raw,
                                            bool use_hostname_suffix_matching);

  RuleList rules_;
};

}  

#endif  
