// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_ID_UTIL_H_
#define EXTENSIONS_COMMON_ID_UTIL_H_

#include <string>

namespace base {
class FilePath;
}

namespace extensions {
namespace id_util {

extern const size_t kIdSize;

std::string GenerateId(const std::string& input);

std::string GenerateIdForPath(const base::FilePath& path);

base::FilePath MaybeNormalizePath(const base::FilePath& path);

}  
}  

#endif  
