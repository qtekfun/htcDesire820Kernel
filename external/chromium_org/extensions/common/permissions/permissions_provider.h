// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_PERMISSIONS_PERMISSIONS_PROVIDER_H_
#define EXTENSIONS_COMMON_PERMISSIONS_PERMISSIONS_PROVIDER_H_

#include <vector>

namespace extensions {

class APIPermissionInfo;

class PermissionsProvider {
 public:
  
  struct AliasInfo {
    const char* name;
    const char* alias;

    AliasInfo(const char* name, const char* alias)
        : name(name), alias(alias) {
    }
  };
  
  
  virtual std::vector<APIPermissionInfo*> GetAllPermissions() const = 0;

  
  virtual std::vector<AliasInfo> GetAllAliases() const = 0;
};

}  

#endif  
