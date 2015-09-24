// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_UTIL_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_UTIL_H_

#include <string>

namespace extensions {
class Extension;
}

class ExtensionService;

namespace extension_util {

bool IsIncognitoEnabled(const std::string& extension_id,
                        const ExtensionService* service);

void SetIsIncognitoEnabled(const std::string& extension_id,
                           ExtensionService* service,
                           bool enabled);

bool CanCrossIncognito(const extensions::Extension* extension,
                       const ExtensionService* service);

bool CanLoadInIncognito(const extensions::Extension* extension,
                        const ExtensionService* service);

bool AllowFileAccess(const extensions::Extension* extension,
                     const ExtensionService* service);

void SetAllowFileAccess(const extensions::Extension* extension,
                        ExtensionService* service,
                        bool allow);

bool IsAppLaunchable(const std::string& extension_id,
                     const ExtensionService* service);

bool IsAppLaunchableWithoutEnabling(const std::string& extension_id,
                                    const ExtensionService* service);

}  

#endif  
