// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_POLICY_NAMESPACE_H_
#define COMPONENTS_POLICY_CORE_COMMON_POLICY_NAMESPACE_H_

#include <string>
#include <vector>

#include "components/policy/policy_export.h"

namespace policy {

enum POLICY_EXPORT PolicyDomain {
  
  POLICY_DOMAIN_CHROME,

  
  
  POLICY_DOMAIN_EXTENSIONS,

  
  POLICY_DOMAIN_SIZE,
};

struct POLICY_EXPORT PolicyNamespace {
 public:
  PolicyNamespace();
  PolicyNamespace(PolicyDomain domain, const std::string& component_id);
  PolicyNamespace(const PolicyNamespace& other);
  ~PolicyNamespace();

  PolicyNamespace& operator=(const PolicyNamespace& other);
  bool operator<(const PolicyNamespace& other) const;
  bool operator==(const PolicyNamespace& other) const;
  bool operator!=(const PolicyNamespace& other) const;

  PolicyDomain domain;
  std::string component_id;
};

typedef std::vector<PolicyNamespace> PolicyNamespaceList;

}  

#endif  
