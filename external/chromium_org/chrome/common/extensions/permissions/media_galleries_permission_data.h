// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_PERMISSIONS_MEDIA_GALLERIES_PERMISSION_DATA_H_
#define CHROME_COMMON_EXTENSIONS_PERMISSIONS_MEDIA_GALLERIES_PERMISSION_DATA_H_

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "extensions/common/permissions/api_permission.h"

namespace base {
class Value;
}

namespace extensions {

class MediaGalleriesPermissionData {
 public:
  MediaGalleriesPermissionData();

  
  
  bool Check(const APIPermission::CheckParam* param) const;

  
  scoped_ptr<base::Value> ToValue() const;

  
  bool FromValue(const base::Value* value);

  bool operator<(const MediaGalleriesPermissionData& rhs) const;
  bool operator==(const MediaGalleriesPermissionData& rhs) const;

  std::string permission() const { return permission_; }

  
  
  std::string& permission() { return permission_; }

 private:
  std::string permission_;
};

}  

#endif  
