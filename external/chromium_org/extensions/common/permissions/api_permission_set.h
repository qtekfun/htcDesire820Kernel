// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_PERMISSIONS_API_PERMISSION_SET_H_
#define EXTENSIONS_COMMON_PERMISSIONS_API_PERMISSION_SET_H_


#include "extensions/common/permissions/api_permission.h"
#include "extensions/common/permissions/base_set_operators.h"

namespace base {
class ListValue;
}  

namespace extensions {

class Extension;
class APIPermissionSet;

template<>
struct BaseSetOperatorsTraits<APIPermissionSet> {
  typedef APIPermission ElementType;
  typedef APIPermission::ID ElementIDType;
};

class APIPermissionSet : public BaseSetOperators<APIPermissionSet> {
 public:
  enum ParseSource {
    
    
    kDisallowInternalPermissions,

    
    
    kAllowInternalPermissions,
  };

  void insert(APIPermission::ID id);

  
  
  void insert(APIPermission* permission);

  
  
  
  
  
  
  
  
  static bool ParseFromJSON(
      const base::ListValue* permissions,
      ParseSource source,
      APIPermissionSet* api_permissions,
      string16* error,
      std::vector<std::string>* unhandled_permissions);

  void AddImpliedPermissions();
};

}  

#endif  
