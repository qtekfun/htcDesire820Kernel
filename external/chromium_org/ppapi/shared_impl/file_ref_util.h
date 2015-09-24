// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SHARED_IMPL_FILE_REF_UTIL_H_
#define PPAPI_SHARED_IMPL_FILE_REF_UTIL_H_

#include <string>

#include "ppapi/shared_impl/ppapi_shared_export.h"

namespace base {
class FilePath;
}

namespace ppapi {

PPAPI_SHARED_EXPORT std::string GetNameForInternalFilePath(
    const std::string& path);
PPAPI_SHARED_EXPORT std::string GetNameForExternalFilePath(
    const base::FilePath& path);

PPAPI_SHARED_EXPORT bool IsValidInternalPath(const std::string& path);

PPAPI_SHARED_EXPORT bool IsValidExternalPath(const base::FilePath& path);

PPAPI_SHARED_EXPORT void NormalizeInternalPath(std::string* path);

}  

#endif  
