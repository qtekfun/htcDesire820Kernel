// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_PERMISSIONS_PERMISSIONS_INFO_H_
#define EXTENSIONS_COMMON_PERMISSIONS_PERMISSIONS_INFO_H_

#include <map>
#include <set>
#include <string>

#include "base/callback.h"
#include "base/lazy_instance.h"
#include "extensions/common/permissions/api_permission.h"
#include "extensions/common/permissions/api_permission_set.h"
#include "extensions/common/permissions/permission_message.h"
#include "extensions/common/permissions/permissions_provider.h"

namespace extensions {

class PermissionsInfo {
 public:
  static PermissionsInfo* GetInstance();

  virtual ~PermissionsInfo();

  
  const APIPermissionInfo* GetByID(APIPermission::ID id) const;

  
  
  const APIPermissionInfo* GetByName(const std::string& name) const;

  
  APIPermissionSet GetAll() const;

  
  APIPermissionSet GetAllByName(
      const std::set<std::string>& permission_names) const;

  
  
  bool HasChildPermissions(const std::string& name) const;

  
  size_t get_permission_count() const { return permission_count_; }

 private:
  friend struct base::DefaultLazyInstanceTraits<PermissionsInfo>;

  PermissionsInfo();

  
  void InitializeWithProvider(const PermissionsProvider& provider);

  
  void RegisterAlias(const char* name, const char* alias);

  
  void RegisterPermission(APIPermissionInfo* permission);

  
  typedef std::map<APIPermission::ID, APIPermissionInfo*> IDMap;

  
  typedef std::map<std::string, APIPermissionInfo*> NameMap;

  IDMap id_map_;
  NameMap name_map_;

  size_t hosted_app_permission_count_;
  size_t permission_count_;

  DISALLOW_COPY_AND_ASSIGN(PermissionsInfo);
};

}  

#endif  
