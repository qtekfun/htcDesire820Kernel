// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_GUID_H_
#define BASE_GUID_H_

#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "build/build_config.h"

namespace base {

BASE_EXPORT std::string GenerateGUID();

BASE_EXPORT bool IsValidGUID(const std::string& guid);

#if defined(OS_POSIX)
BASE_EXPORT std::string RandomDataToGUIDString(const uint64 bytes[2]);
#endif

}  

#endif  
