// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_TEST_FILE_UTIL_H_
#define BASE_TEST_TEST_FILE_UTIL_H_
#pragma once


#include <string>

class FilePath;

namespace file_util {

bool DieFileDie(const FilePath& file, bool recurse);

bool EvictFileFromSystemCache(const FilePath& file);

// destination that already exist will be overwritten.
bool CopyRecursiveDirNoCache(const FilePath& source_dir,
                             const FilePath& dest_dir);

#if defined(OS_WIN)
bool VolumeSupportsADS(const FilePath& path);

bool HasInternetZoneIdentifier(const FilePath& full_path);
#endif  

std::wstring FilePathAsWString(const FilePath& path);
FilePath WStringAsFilePath(const std::wstring& path);

}  

#endif  
