// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_BROWSER_FILEAPI_FILE_PERMISSION_POLICY_H_
#define WEBKIT_BROWSER_FILEAPI_FILE_PERMISSION_POLICY_H_

#include "webkit/browser/webkit_storage_browser_export.h"

namespace fileapi {

enum FilePermissionPolicy {
  
  FILE_PERMISSION_ALWAYS_DENY = 0x0,

  
  FILE_PERMISSION_SANDBOX = 1 << 0,

  
  FILE_PERMISSION_READ_ONLY = 1 << 1,

  
  FILE_PERMISSION_USE_FILE_PERMISSION = 1 << 2,
};

}  

#endif  
