// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_COMMON_EXTENSIONS_PERMISSIONS_SOCKET_PERMISSION_DATA_H_
#define CHROME_COMMON_EXTENSIONS_PERMISSIONS_SOCKET_PERMISSION_DATA_H_

#include <string>

#include "chrome/common/extensions/permissions/socket_permission_entry.h"
#include "extensions/common/permissions/api_permission.h"
#include "ipc/ipc_param_traits.h"

template <class T> struct FuzzTraits;

namespace extensions {

class SocketPermissionData {
 public:
  SocketPermissionData();
  ~SocketPermissionData();

  
  
  bool operator<(const SocketPermissionData& rhs) const;
  bool operator==(const SocketPermissionData& rhs) const;

  
  
  bool Check(const APIPermission::CheckParam* param) const;

  
  scoped_ptr<base::Value> ToValue() const;

  
  bool FromValue(const base::Value* value);

  
  
  
  bool ParseForTest(const std::string& permission) { return Parse(permission); }
  const std::string& GetAsStringForTest() const { return GetAsString(); }

  const SocketPermissionEntry& entry() const { return entry_; }

 private:
  
  friend struct IPC::ParamTraits<SocketPermissionData>;
  friend struct FuzzTraits<SocketPermissionData>;

  SocketPermissionEntry& entry();

  bool Parse(const std::string& permission);
  const std::string& GetAsString() const;
  void Reset();

  SocketPermissionEntry entry_;
  mutable std::string spec_;
};

}  

#endif  
