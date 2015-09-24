// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_PERMISSIONS_CHROME_PERMISSION_MESSAGE_PROVIDER_H_
#define CHROME_COMMON_EXTENSIONS_PERMISSIONS_CHROME_PERMISSION_MESSAGE_PROVIDER_H_

#include <set>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"
#include "extensions/common/permissions/permission_message_provider.h"

namespace extensions {

class ChromePermissionMessageProvider : public PermissionMessageProvider {
 public:
  ChromePermissionMessageProvider();
  virtual ~ChromePermissionMessageProvider();

  
  virtual PermissionMessages GetPermissionMessages(
      const PermissionSet* permissions,
      Manifest::Type extension_type) const OVERRIDE;
  virtual std::vector<base::string16> GetWarningMessages(
      const PermissionSet* permissions,
      Manifest::Type extension_type) const OVERRIDE;
  virtual std::vector<base::string16> GetWarningMessagesDetails(
      const PermissionSet* permissions,
      Manifest::Type extension_type) const OVERRIDE;
  virtual bool IsPrivilegeIncrease(
      const PermissionSet* old_permissions,
      const PermissionSet* new_permissions,
      Manifest::Type extension_type) const OVERRIDE;

 private:
  
  std::set<PermissionMessage> GetAPIPermissionMessages(
      const PermissionSet* permissions) const;

  
  std::set<PermissionMessage> GetManifestPermissionMessages(
      const PermissionSet* permissions) const;

  
  std::set<PermissionMessage> GetHostPermissionMessages(
      const PermissionSet* permissions,
      Manifest::Type extension_type) const;

  
  
  bool IsAPIPrivilegeIncrease(
      const PermissionSet* old_permissions,
      const PermissionSet* new_permissions) const;

  
  
  bool IsManifestPermissionPrivilegeIncrease(
      const PermissionSet* old_permissions,
      const PermissionSet* new_permissions) const;

  
  
  bool IsHostPrivilegeIncrease(
      const PermissionSet* old_permissions,
      const PermissionSet* new_permissions,
      Manifest::Type extension_type) const;

  DISALLOW_COPY_AND_ASSIGN(ChromePermissionMessageProvider);
};

}  

#endif  
