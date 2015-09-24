// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_SUGGEST_PERMISSION_UTIL_H_
#define CHROME_BROWSER_EXTENSIONS_SUGGEST_PERMISSION_UTIL_H_

#include "extensions/common/permissions/api_permission.h"

class Profile;

namespace content {
class RenderViewHost;
}

namespace extensions {

class Extension;

void SuggestAPIPermissionInDevToolsConsole(APIPermission::ID permission,
                                           const Extension* extension,
                                           content::RenderViewHost* host);

void SuggestAPIPermissionInDevToolsConsole(APIPermission::ID permission,
                                           const Extension* extension,
                                           Profile* profile);

bool IsExtensionWithPermissionOrSuggestInConsole(
    APIPermission::ID permission,
    const Extension* extension,
    content::RenderViewHost* host);

bool IsExtensionWithPermissionOrSuggestInConsole(
    APIPermission::ID permission,
    const Extension* extension,
    Profile* profile);

}  

#endif  
