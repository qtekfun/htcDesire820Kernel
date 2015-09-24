// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_PEPPER_PERMISSION_UTIL_H_
#define CHROME_COMMON_PEPPER_PERMISSION_UTIL_H_

#include <set>
#include <string>

class ExtensionSet;
class GURL;

namespace chrome {

bool IsExtensionOrSharedModuleWhitelisted(
    const GURL& url,
    const ExtensionSet* extension_set,
    const std::set<std::string>& whitelist);

bool IsHostAllowedByCommandLine(const GURL& url,
                                const ExtensionSet* extension_set,
                                const char* command_line_switch);
}  

#endif  
