// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_PERMISSIONS_PERMISSIONS_API_HELPERS_H_
#define CHROME_BROWSER_EXTENSIONS_API_PERMISSIONS_PERMISSIONS_API_HELPERS_H_

#include <string>
#include "base/memory/scoped_ptr.h"
#include "base/memory/ref_counted.h"

namespace base {
class DictionaryValue;
}

namespace extensions {

class PermissionSet;

namespace api {
namespace permissions {
struct Permissions;
}
}

namespace permissions_api_helpers {

scoped_ptr<api::permissions::Permissions> PackPermissionSet(
    const PermissionSet* set);

scoped_refptr<PermissionSet> UnpackPermissionSet(
    const api::permissions::Permissions& permissions,
    bool allow_file_access,
    std::string* error);

}  
}  

#endif  
