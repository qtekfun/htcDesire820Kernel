// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_PLATFORM_FILE_H_
#define BASE_PLATFORM_FILE_H_
#pragma once

#include "build/build_config.h"
#if defined(OS_WIN)
#include <windows.h>
#endif

#include <string>

#include "base/base_api.h"
#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/time.h"

namespace base {

#if defined(OS_WIN)
typedef HANDLE PlatformFile;
const PlatformFile kInvalidPlatformFileValue = INVALID_HANDLE_VALUE;
#elif defined(OS_POSIX)
typedef int PlatformFile;
const PlatformFile kInvalidPlatformFileValue = -1;
#endif

enum PlatformFileFlags {
  PLATFORM_FILE_OPEN = 1,
  PLATFORM_FILE_CREATE = 2,
  PLATFORM_FILE_OPEN_ALWAYS = 4,    
  PLATFORM_FILE_CREATE_ALWAYS = 8,  
  PLATFORM_FILE_READ = 16,
  PLATFORM_FILE_WRITE = 32,
  PLATFORM_FILE_EXCLUSIVE_READ = 64,  
  PLATFORM_FILE_EXCLUSIVE_WRITE = 128,
  PLATFORM_FILE_ASYNC = 256,
  PLATFORM_FILE_TEMPORARY = 512,        
  PLATFORM_FILE_HIDDEN = 1024,          
  PLATFORM_FILE_DELETE_ON_CLOSE = 2048,
  PLATFORM_FILE_TRUNCATE = 4096,
  PLATFORM_FILE_WRITE_ATTRIBUTES = 8192,  
  PLATFORM_FILE_ENUMERATE = 16384,  
};

enum PlatformFileError {
  PLATFORM_FILE_OK = 0,
  PLATFORM_FILE_ERROR_FAILED = -1,
  PLATFORM_FILE_ERROR_IN_USE = -2,
  PLATFORM_FILE_ERROR_EXISTS = -3,
  PLATFORM_FILE_ERROR_NOT_FOUND = -4,
  PLATFORM_FILE_ERROR_ACCESS_DENIED = -5,
  PLATFORM_FILE_ERROR_TOO_MANY_OPENED = -6,
  PLATFORM_FILE_ERROR_NO_MEMORY = -7,
  PLATFORM_FILE_ERROR_NO_SPACE = -8,
  PLATFORM_FILE_ERROR_NOT_A_DIRECTORY = -9,
  PLATFORM_FILE_ERROR_INVALID_OPERATION = -10,
  PLATFORM_FILE_ERROR_SECURITY = -11,
  PLATFORM_FILE_ERROR_ABORT = -12,
  PLATFORM_FILE_ERROR_NOT_A_FILE = -13,
  PLATFORM_FILE_ERROR_NOT_EMPTY = -14,
  PLATFORM_FILE_ERROR_INVALID_URL = -15,
};

struct BASE_API PlatformFileInfo {
  PlatformFileInfo();
  ~PlatformFileInfo();

  
  int64 size;

  
  bool is_directory;

  
  bool is_symbolic_link;

  
  base::Time last_modified;

  
  base::Time last_accessed;

  
  base::Time creation_time;
};

BASE_API PlatformFile CreatePlatformFile(const FilePath& name,
                                         int flags,
                                         bool* created,
                                         PlatformFileError* error_code);

BASE_API bool ClosePlatformFile(PlatformFile file);

BASE_API int ReadPlatformFile(PlatformFile file, int64 offset,
                              char* data, int size);

// data that was previously there. Returns the number of bytes written, or -1
BASE_API int WritePlatformFile(PlatformFile file, int64 offset,
                               const char* data, int size);

BASE_API bool TruncatePlatformFile(PlatformFile file, int64 length);

BASE_API bool FlushPlatformFile(PlatformFile file);

BASE_API bool TouchPlatformFile(PlatformFile file, const Time& last_access_time,
                                const Time& last_modified_time);

BASE_API bool GetPlatformFileInfo(PlatformFile file, PlatformFileInfo* info);

class BASE_API PassPlatformFile {
 public:
  explicit PassPlatformFile(PlatformFile* value) : value_(value) {
  }

  
  
  
  PlatformFile ReleaseValue() {
    PlatformFile temp = *value_;
    *value_ = kInvalidPlatformFileValue;
    return temp;
  }

 private:
  PlatformFile* value_;
};

}  

#endif  
