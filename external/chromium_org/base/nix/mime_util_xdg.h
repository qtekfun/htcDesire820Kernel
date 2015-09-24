// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_NIX_MIME_UTIL_XDG_H_
#define BASE_NIX_MIME_UTIL_XDG_H_

#include <string>

#include "base/base_export.h"
#include "build/build_config.h"

namespace base {

class FilePath;

namespace nix {

BASE_EXPORT std::string GetFileMimeType(const FilePath& filepath);

BASE_EXPORT std::string GetDataMimeType(const std::string& data);

BASE_EXPORT void SetIconThemeName(const std::string& name);

BASE_EXPORT FilePath GetMimeIcon(const std::string& mime_type, size_t size);

}  
}  

#endif  
