// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_PERMISSIONS_PERMISSION_SET_H_
#define EXTENSIONS_COMMON_PERMISSIONS_PERMISSION_SET_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/singleton.h"
#include "base/strings/string16.h"
#include "extensions/common/manifest.h"
#include "extensions/common/permissions/api_permission.h"
#include "extensions/common/permissions/api_permission_set.h"
#include "extensions/common/permissions/manifest_permission.h"
#include "extensions/common/permissions/manifest_permission_set.h"
#include "extensions/common/url_pattern_set.h"

namespace extensions {
class Extension;

class PermissionSet
    : public base::RefCountedThreadSafe<PermissionSet> {
 public:
  
  PermissionSet();

  
  
  
  
  PermissionSet(const APIPermissionSet& apis,
                const ManifestPermissionSet& manifest_permissions,
                const URLPatternSet& explicit_hosts,
                const URLPatternSet& scriptable_hosts);

  
  
  static PermissionSet* CreateDifference(
      const PermissionSet* set1, const PermissionSet* set2);

  
  
  static PermissionSet* CreateIntersection(
      const PermissionSet* set1, const PermissionSet* set2);

  
  
  static PermissionSet* CreateUnion(
      const PermissionSet* set1, const PermissionSet* set2);

  bool operator==(const PermissionSet& rhs) const;

  
  
  
  bool Contains(const PermissionSet& set) const;

  
  std::set<std::string> GetAPIsAsStrings() const;

  
  bool IsEmpty() const;

  
  bool HasAPIPermission(APIPermission::ID permission) const;

  
  
  
  bool HasAPIPermission(const std::string& permission_name) const;

  
  
  bool CheckAPIPermission(APIPermission::ID permission) const;

  
  bool CheckAPIPermissionWithParam(APIPermission::ID permission,
      const APIPermission::CheckParam* param) const;

  
  bool HasExplicitAccessToOrigin(const GURL& origin) const;

  
  bool HasScriptableAccessToURL(const GURL& url) const;

  
  
  bool HasEffectiveAccessToAllHosts() const;

  
  bool HasEffectiveAccessToURL(const GURL& url) const;

  
  
  bool HasEffectiveFullAccess() const;

  const APIPermissionSet& apis() const { return apis_; }

  const ManifestPermissionSet& manifest_permissions() const {
      return manifest_permissions_;
  }

  const URLPatternSet& effective_hosts() const { return effective_hosts_; }

  const URLPatternSet& explicit_hosts() const { return explicit_hosts_; }

  const URLPatternSet& scriptable_hosts() const { return scriptable_hosts_; }

 private:
  FRIEND_TEST_ALL_PREFIXES(PermissionsTest, GetWarningMessages_AudioVideo);
  friend class base::RefCountedThreadSafe<PermissionSet>;

  ~PermissionSet();

  void AddAPIPermission(APIPermission::ID id);

  
  void InitImplicitPermissions();

  
  void InitEffectiveHosts();

  
  
  APIPermissionSet apis_;

  
  
  ManifestPermissionSet manifest_permissions_;

  
  
  URLPatternSet explicit_hosts_;

  
  
  URLPatternSet scriptable_hosts_;

  
  URLPatternSet effective_hosts_;
};

}  

#endif  
