// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_GUID_H_
#define CHROME_COMMON_GUID_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "build/build_config.h"

namespace guid {

std::string GenerateGUID();

bool IsValidGUID(const std::string& guid);

#if defined(OS_POSIX)
std::string RandomDataToGUIDString(const uint64 bytes[2]);
#endif

}  

#endif  
