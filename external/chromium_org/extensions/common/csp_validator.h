// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_CSP_VALIDATOR_H_
#define EXTENSIONS_COMMON_CSP_VALIDATOR_H_

#include <string>

#include "extensions/common/manifest.h"

namespace extensions {

namespace csp_validator {

bool ContentSecurityPolicyIsLegal(const std::string& policy);

bool ContentSecurityPolicyIsSecure(
    const std::string& policy, Manifest::Type type);

bool ContentSecurityPolicyIsSandboxed(
    const std::string& policy, Manifest::Type type);

}  

}  

#endif  
