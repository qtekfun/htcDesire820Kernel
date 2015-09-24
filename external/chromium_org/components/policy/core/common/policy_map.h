// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_POLICY_MAP_H_
#define COMPONENTS_POLICY_CORE_COMMON_POLICY_MAP_H_

#include <map>
#include <set>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "components/policy/core/common/external_data_fetcher.h"
#include "components/policy/core/common/policy_types.h"
#include "components/policy/policy_export.h"

namespace policy {

class POLICY_EXPORT PolicyMap {
 public:
  
  
  struct POLICY_EXPORT Entry {
    PolicyLevel level;
    PolicyScope scope;
    base::Value* value;
    ExternalDataFetcher* external_data_fetcher;

    Entry();

    
    void DeleteOwnedMembers();

    
    scoped_ptr<Entry> DeepCopy() const;

    
    bool has_higher_priority_than(const Entry& other) const;

    
    bool Equals(const Entry& other) const;
  };

  typedef std::map<std::string, Entry> PolicyMapType;
  typedef PolicyMapType::const_iterator const_iterator;

  PolicyMap();
  virtual ~PolicyMap();

  
  
  const Entry* Get(const std::string& policy) const;

  
  
  
  const base::Value* GetValue(const std::string& policy) const;

  
  
  void Set(const std::string& policy,
           PolicyLevel level,
           PolicyScope scope,
           base::Value* value,
           ExternalDataFetcher* external_data_fetcher);

  
  void Erase(const std::string& policy);

  
  void Swap(PolicyMap* other);

  
  void CopyFrom(const PolicyMap& other);

  
  scoped_ptr<PolicyMap> DeepCopy() const;

  
  
  
  
  void MergeFrom(const PolicyMap& other);

  
  
  
  void LoadFrom(const base::DictionaryValue* policies,
                PolicyLevel level,
                PolicyScope scope);

  
  
  
  
  void GetDifferingKeys(const PolicyMap& other,
                        std::set<std::string>* differing_keys) const;

  
  
  
  
  void FilterLevel(PolicyLevel level);

  bool Equals(const PolicyMap& other) const;
  bool empty() const;
  size_t size() const;

  const_iterator begin() const;
  const_iterator end() const;
  void Clear();

 private:
  
  static bool MapEntryEquals(const PolicyMapType::value_type& a,
                             const PolicyMapType::value_type& b);

  PolicyMapType map_;

  DISALLOW_COPY_AND_ASSIGN(PolicyMap);
};

}  

#endif  
