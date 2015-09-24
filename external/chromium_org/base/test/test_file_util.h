// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_TEST_FILE_UTIL_H_
#define BASE_TEST_TEST_FILE_UTIL_H_


#include <string>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"

#if defined(OS_ANDROID)
#include <jni.h>
#include "base/basictypes.h"
#endif

namespace base {

class FilePath;

bool EvictFileFromSystemCacheWithRetry(const FilePath& file);

}  

namespace file_util {

bool DieFileDie(const base::FilePath& file, bool recurse);

bool EvictFileFromSystemCache(const base::FilePath& file);

#if defined(OS_WIN)
bool VolumeSupportsADS(const base::FilePath& path);

bool HasInternetZoneIdentifier(const base::FilePath& full_path);
#endif  

std::wstring FilePathAsWString(const base::FilePath& path);
base::FilePath WStringAsFilePath(const std::wstring& path);

bool MakeFileUnreadable(const base::FilePath& path) WARN_UNUSED_RESULT;
bool MakeFileUnwritable(const base::FilePath& path) WARN_UNUSED_RESULT;

#if defined(OS_ANDROID)
bool RegisterContentUriTestUtils(JNIEnv* env);

base::FilePath InsertImageIntoMediaStore(const base::FilePath& path);
#endif  

class PermissionRestorer {
 public:
  explicit PermissionRestorer(const base::FilePath& path);
  ~PermissionRestorer();

 private:
  const base::FilePath path_;
  void* info_;  
  size_t length_;  

  DISALLOW_COPY_AND_ASSIGN(PermissionRestorer);
};

}  

#endif  
