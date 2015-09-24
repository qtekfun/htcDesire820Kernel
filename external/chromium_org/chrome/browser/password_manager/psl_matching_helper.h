// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PSL_MATCHING_HELPER_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PSL_MATCHING_HELPER_H_

#include <string>

#include "base/basictypes.h"

class GURL;

namespace autofill {
struct PasswordForm;
}  

class PSLMatchingHelper {
 public:
  
  
  
  enum PSLDomainMatchMetric {
    PSL_DOMAIN_MATCH_DISABLED = 0,
    PSL_DOMAIN_MATCH_NONE,
    PSL_DOMAIN_MATCH_FOUND,
    PSL_DOMAIN_MATCH_COUNT
  };

  PSLMatchingHelper();
  ~PSLMatchingHelper();

  bool IsMatchingEnabled() const;

  
  
  
  
  
  
  
  
  
  
  
  bool ShouldPSLDomainMatchingApply(
      const std::string& registry_controlled_domain) const;

  
  
  static bool IsPublicSuffixDomainMatch(const std::string& url1,
                                        const std::string& url2);

  
  
  static std::string GetRegistryControlledDomain(const GURL& signon_realm);

  
  
  
  static void EnablePublicSuffixDomainMatchingForTesting();

 private:
  static bool DeterminePSLEnabled();

  const bool psl_enabled_;

  
  static bool psl_enabled_override_;

  DISALLOW_COPY_AND_ASSIGN(PSLMatchingHelper);
};

#endif  
