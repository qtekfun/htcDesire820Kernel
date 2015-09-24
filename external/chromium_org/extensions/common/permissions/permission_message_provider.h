// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_PERMISSIONS_PERMISSION_MESSAGE_PROVIDER_H_
#define EXTENSIONS_COMMON_PERMISSIONS_PERMISSION_MESSAGE_PROVIDER_H_

#include <vector>

#include "extensions/common/manifest.h"
#include "extensions/common/permissions/permission_message.h"

namespace extensions {

class PermissionSet;

class PermissionMessageProvider {
 public:
  PermissionMessageProvider() {}
  virtual ~PermissionMessageProvider() {}

  
  static const PermissionMessageProvider* Get();

  
  
  virtual PermissionMessages GetPermissionMessages(
      const PermissionSet* permissions,
      Manifest::Type extension_type) const = 0;

  
  
  virtual std::vector<string16> GetWarningMessages(
      const PermissionSet* permissions,
      Manifest::Type extension_type) const = 0;

  
  
  
  virtual std::vector<string16> GetWarningMessagesDetails(
      const PermissionSet* permissions,
      Manifest::Type extension_type) const = 0;

  
  
  
  virtual bool IsPrivilegeIncrease(
      const PermissionSet* old_permissions,
      const PermissionSet* new_permissions,
      Manifest::Type extension_type) const = 0;
};

}  

#endif  
