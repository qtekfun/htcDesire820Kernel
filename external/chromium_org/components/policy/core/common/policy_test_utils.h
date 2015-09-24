// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_POLICY_CORE_COMMON_POLICY_TEST_UTILS_H_
#define COMPONENTS_POLICY_CORE_COMMON_POLICY_TEST_UTILS_H_

#include <map>
#include <ostream>
#include <string>

#include "base/basictypes.h"
#include "components/policy/core/common/policy_details.h"
#include "components/policy/core/common/policy_map.h"
#include "components/policy/core/common/policy_service.h"
#include "components/policy/core/common/policy_types.h"

namespace policy {

class PolicyBundle;
struct PolicyNamespace;

class PolicyDetailsMap {
 public:
  PolicyDetailsMap();
  ~PolicyDetailsMap();

  
  GetChromePolicyDetailsCallback GetCallback() const;

  
  void SetDetails(const std::string& policy, const PolicyDetails* details);

 private:
  typedef std::map<std::string, const PolicyDetails*> PolicyDetailsMapping;

  const PolicyDetails* Lookup(const std::string& policy) const;

  PolicyDetailsMapping map_;

  DISALLOW_COPY_AND_ASSIGN(PolicyDetailsMap);
};

bool PolicyServiceIsEmpty(const PolicyService* service);

}  

std::ostream& operator<<(std::ostream& os, const policy::PolicyBundle& bundle);
std::ostream& operator<<(std::ostream& os, policy::PolicyScope scope);
std::ostream& operator<<(std::ostream& os, policy::PolicyLevel level);
std::ostream& operator<<(std::ostream& os, policy::PolicyDomain domain);
std::ostream& operator<<(std::ostream& os, const policy::PolicyMap& policies);
std::ostream& operator<<(std::ostream& os, const policy::PolicyMap::Entry& e);
std::ostream& operator<<(std::ostream& os, const policy::PolicyNamespace& ns);

#endif  
