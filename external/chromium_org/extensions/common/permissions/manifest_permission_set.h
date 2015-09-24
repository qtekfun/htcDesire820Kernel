// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_PERMISSIONS_MANIFEST_PERMISSION_SET_H_
#define EXTENSIONS_COMMON_PERMISSIONS_MANIFEST_PERMISSION_SET_H_

#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "extensions/common/permissions/base_set_operators.h"

namespace base {
class ListValue;
}  

namespace extensions {

class Extension;
class ManifestPermission;
class ManifestPermissionSet;

template<>
struct BaseSetOperatorsTraits<ManifestPermissionSet> {
  typedef ManifestPermission ElementType;
  typedef std::string ElementIDType;
};

class ManifestPermissionSet : public BaseSetOperators<ManifestPermissionSet> {
 public:
  
  
  
  
  
  
  static bool ParseFromJSON(
      const base::ListValue* permissions,
      ManifestPermissionSet* manifest_permissions,
      string16* error,
      std::vector<std::string>* unhandled_permissions);
};

}  

#endif  
