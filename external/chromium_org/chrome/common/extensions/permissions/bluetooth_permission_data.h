// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_PERMISSIONS_BLUETOOTH_PERMISSION_DATA_H_
#define CHROME_COMMON_EXTENSIONS_PERMISSIONS_BLUETOOTH_PERMISSION_DATA_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "extensions/common/permissions/api_permission.h"

namespace base {

class Value;

}  

namespace extensions {

class BluetoothPermissionData {
 public:
  BluetoothPermissionData();
  explicit BluetoothPermissionData(const std::string& uuid);

  
  
  bool Check(const APIPermission::CheckParam* param) const;

  
  scoped_ptr<base::Value> ToValue() const;

  
  bool FromValue(const base::Value* value);

  bool operator<(const BluetoothPermissionData& rhs) const;
  bool operator==(const BluetoothPermissionData& rhs) const;

  
  const std::string& uuid() const { return uuid_; }

  
  
  std::string& uuid() { return uuid_; }

 private:
  std::string uuid_;
};

}  

#endif  
