// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WEBKIT_COMMON_FILEAPI_FILE_SYSTEM_TYPES_H_
#define WEBKIT_COMMON_FILEAPI_FILE_SYSTEM_TYPES_H_

#include "third_party/WebKit/public/platform/WebFileSystemType.h"

namespace fileapi {

enum FileSystemType {
  
  kFileSystemTypeUnknown = -1,

  
  
  

  
  
  
  
  
  
  
  kFileSystemTypeTemporary = blink::WebFileSystemTypeTemporary,
  kFileSystemTypePersistent = blink::WebFileSystemTypePersistent,

  
  kFileSystemTypeIsolated = blink::WebFileSystemTypeIsolated,

  
  
  
  
  
  
  kFileSystemTypeExternal = blink::WebFileSystemTypeExternal,

  
  
  kFileSystemInternalTypeEnumStart = 99,

  
  
  
  
  
  
  
  

  
  kFileSystemTypeTest,

  
  
  kFileSystemTypeNativeLocal,

  
  
  
  kFileSystemTypeRestrictedNativeLocal,

  
  
  kFileSystemTypeDragged,

  
  
  kFileSystemTypeNativeMedia,

  
  
  kFileSystemTypeDeviceMedia,

  
  kFileSystemTypePicasa,

  
  kFileSystemTypeItunes,

  
  kFileSystemTypeIphoto,

  
  kFileSystemTypeDrive,

  
  
  kFileSystemTypeSyncable,

  
  
  
  
  
  kFileSystemTypeSyncableForInternalSync,

  
  
  
  
  kFileSystemTypeNativeForPlatformApp,

  
  
  
  
  
  kFileSystemTypeForTransientFile,

  
  
  
  kFileSystemTypePluginPrivate,

  
  
  
  kFileSystemInternalTypeEnumEnd,
};

}  

#endif  
