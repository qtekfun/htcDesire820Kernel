// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_PERMISSIONS_MEDIA_GALLERIES_PERMISSION_H_
#define CHROME_COMMON_EXTENSIONS_PERMISSIONS_MEDIA_GALLERIES_PERMISSION_H_

#include "base/basictypes.h"
#include "chrome/common/extensions/permissions/media_galleries_permission_data.h"
#include "chrome/common/extensions/permissions/set_disjunction_permission.h"
#include "extensions/common/permissions/api_permission.h"

namespace extensions {

class MediaGalleriesPermission
  : public SetDisjunctionPermission<MediaGalleriesPermissionData,
                                    MediaGalleriesPermission> {
 public:
  struct CheckParam : public APIPermission::CheckParam {
    explicit CheckParam(const std::string& permission)
      : permission(permission) {}
    const std::string permission;
  };

  explicit MediaGalleriesPermission(const APIPermissionInfo* info);
  virtual ~MediaGalleriesPermission();

  
  
  
  virtual bool FromValue(const base::Value* value) OVERRIDE;

  
  virtual PermissionMessages GetMessages() const OVERRIDE;

  
  static const char kAllAutoDetectedPermission[];
  static const char kReadPermission[];
  static const char kCopyToPermission[];
  static const char kDeletePermission[];
};

}  

#endif  
