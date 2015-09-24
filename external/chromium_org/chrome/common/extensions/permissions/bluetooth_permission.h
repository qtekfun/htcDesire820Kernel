// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_PERMISSIONS_BLUETOOTH_PERMISSION_H_
#define CHROME_COMMON_EXTENSIONS_PERMISSIONS_BLUETOOTH_PERMISSION_H_

#include <string>

#include "chrome/common/extensions/permissions/bluetooth_permission_data.h"
#include "chrome/common/extensions/permissions/set_disjunction_permission.h"
#include "extensions/common/permissions/api_permission.h"

namespace extensions {

class BluetoothPermission
  : public SetDisjunctionPermission<BluetoothPermissionData,
                                    BluetoothPermission> {
 public:
  
  struct CheckParam : APIPermission::CheckParam {
    explicit CheckParam(std::string uuid)
      : uuid(uuid) {}
    const std::string uuid;
  };

  explicit BluetoothPermission(const APIPermissionInfo* info);
  virtual ~BluetoothPermission();

  
  
  
  virtual bool FromValue(const base::Value* value) OVERRIDE;

  
  virtual PermissionMessages GetMessages() const OVERRIDE;
};

}  

#endif  
