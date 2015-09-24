// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_COMMON_USER_AGENT_USER_AGENT_UTIL_H_
#define WEBKIT_COMMON_USER_AGENT_USER_AGENT_UTIL_H_

#include <string>

#include "base/basictypes.h"
#include "webkit/common/user_agent/webkit_user_agent_export.h"

namespace webkit_glue {

WEBKIT_USER_AGENT_EXPORT std::string BuildOSCpuInfo();

WEBKIT_USER_AGENT_EXPORT std::string GetWebKitVersion();

WEBKIT_USER_AGENT_EXPORT int GetWebKitMajorVersion();
WEBKIT_USER_AGENT_EXPORT int GetWebKitMinorVersion();

WEBKIT_USER_AGENT_EXPORT std::string GetWebKitRevision();

WEBKIT_USER_AGENT_EXPORT std::string BuildUserAgentFromProduct(
    const std::string& product);

WEBKIT_USER_AGENT_EXPORT std::string BuildUserAgentFromOSAndProduct(
    const std::string& os_info,
    const std::string& product);

}  

#endif  
