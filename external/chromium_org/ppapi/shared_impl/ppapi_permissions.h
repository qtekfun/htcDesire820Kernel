// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_PPAPI_PERMISSIONS_H_
#define PPAPI_SHARED_IMPL_PPAPI_PERMISSIONS_H_

#include "base/basictypes.h"
#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace ppapi {

enum Permission {
  
  PERMISSION_NONE = 0,

  
  PERMISSION_DEV = 1 << 0,

  
  PERMISSION_PRIVATE = 1 << 1,

  
  
  PERMISSION_BYPASS_USER_GESTURE = 1 << 2,

  
  PERMISSION_TESTING = 1 << 3,

  
  PERMISSION_FLASH = 1 << 4,

  

  
  
  PERMISSION_ALL_BITS = PERMISSION_DEV |
                        PERMISSION_PRIVATE |
                        PERMISSION_BYPASS_USER_GESTURE |
                        PERMISSION_TESTING |
                        PERMISSION_FLASH
};

class PPAPI_SHARED_EXPORT PpapiPermissions {
 public:
  
  PpapiPermissions();

  
  explicit PpapiPermissions(uint32 perms);

  ~PpapiPermissions();

  
  
  static PpapiPermissions AllPermissions();

  
  
  
  static PpapiPermissions GetForCommandLine(uint32 base_perms);

  bool HasPermission(Permission perm) const;

  
  uint32 GetBits() const { return permissions_; }

 private:
  uint32 permissions_;

  
};

}  

#endif  
