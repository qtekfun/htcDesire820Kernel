// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef BASE_FILE_UTIL_DEPRECATED_H_
#define BASE_FILE_UTIL_DEPRECATED_H_
#pragma once

#include "base/base_api.h"
#include "build/build_config.h"


#if defined(OS_WIN)

namespace file_util {

BASE_API FILE* OpenFile(const std::string& filename, const char* mode);
BASE_API FILE* OpenFile(const std::wstring& filename, const char* mode);

BASE_API void AppendToPath(std::wstring* path, const std::wstring& new_ending);

BASE_API FilePath::StringType GetFileExtensionFromPath(const FilePath& path);
BASE_API std::wstring GetFileExtensionFromPath(const std::wstring& path);

BASE_API bool Delete(const std::wstring& path, bool recursive);
BASE_API bool CopyDirectory(const std::wstring& from_path,
                            const std::wstring& to_path,
                            bool recursive);
BASE_API int ReadFile(const std::wstring& filename, char* data, int size);
BASE_API int WriteFile(const std::wstring& filename,
                       const char* data, int size);

}

#endif  

#endif  
