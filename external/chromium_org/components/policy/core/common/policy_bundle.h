// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_POLICY_BUNDLE_H_
#define COMPONENTS_POLICY_CORE_COMMON_POLICY_BUNDLE_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "components/policy/core/common/policy_map.h"
#include "components/policy/core/common/policy_namespace.h"
#include "components/policy/policy_export.h"

namespace policy {

class POLICY_EXPORT PolicyBundle {
 public:
  typedef std::map<PolicyNamespace, PolicyMap*> MapType;
  typedef MapType::iterator iterator;
  typedef MapType::const_iterator const_iterator;

  PolicyBundle();
  virtual ~PolicyBundle();

  
  PolicyMap& Get(const PolicyNamespace& ns);
  const PolicyMap& Get(const PolicyNamespace& ns) const;

  
  void Swap(PolicyBundle* other);

  
  void CopyFrom(const PolicyBundle& other);

  
  
  
  
  
  
  void MergeFrom(const PolicyBundle& other);

  
  bool Equals(const PolicyBundle& other) const;

  
  iterator begin() { return policy_bundle_.begin(); }
  iterator end() { return policy_bundle_.end(); }

  
  
  const_iterator begin() const { return policy_bundle_.begin(); }
  const_iterator end() const { return policy_bundle_.end(); }

  
  void Clear();

 private:
  MapType policy_bundle_;

  
  
  const PolicyMap kEmpty_;

  DISALLOW_COPY_AND_ASSIGN(PolicyBundle);
};

}  

#endif  
