// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_PERMISSIONS_PERMISSIONS_DATA_H_
#define EXTENSIONS_COMMON_PERMISSIONS_PERMISSIONS_DATA_H_

#include <map>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "base/synchronization/lock.h"
#include "extensions/common/permissions/api_permission.h"
#include "extensions/common/permissions/permission_message.h"

class GURL;

namespace extensions {

class PermissionSet;
class APIPermissionSet;
class Extension;
class ManifestPermissionSet;
class URLPatternSet;
class UserScript;

class PermissionsData {
 public:
  PermissionsData();
  ~PermissionsData();

  
  
  class PolicyDelegate {
   public:
    virtual ~PolicyDelegate() {}

    
    
    virtual bool CanExecuteScriptOnPage(const Extension* extension,
                                        const GURL& document_url,
                                        const GURL& top_document_url,
                                        int tab_id,
                                        const UserScript* script,
                                        int process_id,
                                        std::string* error) = 0;
  };

  static void SetPolicyDelegate(PolicyDelegate* delegate);

  
  static const PermissionSet* GetOptionalPermissions(
      const Extension* extension);
  static const PermissionSet* GetRequiredPermissions(
      const Extension* extension);

  
  
  static const APIPermissionSet* GetInitialAPIPermissions(
      const Extension* extension);
  static APIPermissionSet* GetInitialAPIPermissions(Extension* extension);

  
  static void SetInitialScriptableHosts(Extension* extension,
                                        const URLPatternSet& scriptable_hosts);

  
  static scoped_refptr<const PermissionSet> GetActivePermissions(
      const Extension* extension);
  
  static void SetActivePermissions(const Extension* extension,
                                   const PermissionSet* active);

  
  
  static scoped_refptr<const PermissionSet> GetTabSpecificPermissions(
      const Extension* extension,
      int tab_id);
  
  
  static void UpdateTabSpecificPermissions(
      const Extension* extension,
      int tab_id,
      scoped_refptr<const PermissionSet> permissions);
  
  static void ClearTabSpecificPermissions(const Extension* extension,
                                          int tab_id);

  
  
  
  
  
  
  static bool HasAPIPermission(const Extension* extension,
                               APIPermission::ID permission);
  static bool HasAPIPermission(const Extension* extension,
                               const std::string& permission_name);
  static bool HasAPIPermissionForTab(const Extension* extension,
                                     int tab_id,
                                     APIPermission::ID permission);

  static bool CheckAPIPermissionWithParam(
      const Extension* extension,
      APIPermission::ID permission,
      const APIPermission::CheckParam* param);

  static const URLPatternSet& GetEffectiveHostPermissions(
      const Extension* extension);

  
  
  
  
  
  
  static bool CanSilentlyIncreasePermissions(const Extension* extension);

  
  
  static bool ShouldSkipPermissionWarnings(const Extension* extension);

  
  static bool HasHostPermission(const Extension* extension, const GURL& url);

  
  
  
  
  
  static bool HasEffectiveAccessToAllHosts(const Extension* extension);

  
  
  static PermissionMessages GetPermissionMessages(const Extension* extension);
  
  
  
  static std::vector<string16> GetPermissionMessageStrings(
      const Extension* extension);

  
  
  
  static std::vector<string16> GetPermissionMessageDetailsStrings(
      const Extension* extension);

  
  
  
  
  
  
  
  static bool CanExecuteScriptOnPage(const Extension* extension,
                                     const GURL& document_url,
                                     const GURL& top_document_url,
                                     int tab_id,
                                     const UserScript* script,
                                     int process_id,
                                     std::string* error);

  
  
  static bool CanExecuteScriptEverywhere(const Extension* extension);

  
  
  
  
  static bool CanCaptureVisiblePage(const Extension* extension,
                                    const GURL& page_url,
                                    int tab_id,
                                    std::string* error);

  
  bool ParsePermissions(Extension* extension, string16* error);

  
  void InitializeManifestPermissions(Extension* extension);

  
  void FinalizePermissions(Extension* extension);

 private:
  struct InitialPermissions;
  typedef std::map<int, scoped_refptr<const PermissionSet> > TabPermissionsMap;

  
  
  scoped_ptr<InitialPermissions> initial_required_permissions_;
  scoped_ptr<InitialPermissions> initial_optional_permissions_;

  
  scoped_refptr<const PermissionSet> optional_permission_set_;

  
  scoped_refptr<const PermissionSet> required_permission_set_;

  mutable base::Lock runtime_lock_;

  
  
  mutable scoped_refptr<const PermissionSet> active_permissions_;

  mutable TabPermissionsMap tab_specific_permissions_;

  DISALLOW_COPY_AND_ASSIGN(PermissionsData);
};

}  

#endif  
