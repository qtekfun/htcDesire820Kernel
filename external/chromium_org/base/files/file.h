// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILES_FILE_H_
#define BASE_FILES_FILE_H_

#include "build/build_config.h"
#if defined(OS_WIN)
#include <windows.h>
#endif

#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/move.h"
#include "base/time/time.h"

#if defined(OS_WIN)
#include "base/win/scoped_handle.h"
#endif

namespace base {

#if defined(OS_WIN)
typedef HANDLE PlatformFile;
#elif defined(OS_POSIX)
typedef int PlatformFile;
#endif


class BASE_EXPORT File {
  MOVE_ONLY_TYPE_FOR_CPP_03(File, RValue)

 public:
  
  
  
  
  
  
  
  enum Flags {
    FLAG_OPEN = 1 << 0,             
    FLAG_CREATE = 1 << 1,           
                                    
    FLAG_OPEN_ALWAYS = 1 << 2,      
    FLAG_CREATE_ALWAYS = 1 << 3,    
    FLAG_OPEN_TRUNCATED = 1 << 4,   
                                    
    FLAG_READ = 1 << 5,
    FLAG_WRITE = 1 << 6,
    FLAG_APPEND = 1 << 7,
    FLAG_EXCLUSIVE_READ = 1 << 8,   
    FLAG_EXCLUSIVE_WRITE = 1 << 9,
    FLAG_ASYNC = 1 << 10,
    FLAG_TEMPORARY = 1 << 11,       
    FLAG_HIDDEN = 1 << 12,          
    FLAG_DELETE_ON_CLOSE = 1 << 13,
    FLAG_WRITE_ATTRIBUTES = 1 << 14,  
    FLAG_SHARE_DELETE = 1 << 15,      
    FLAG_TERMINAL_DEVICE = 1 << 16,   
    FLAG_BACKUP_SEMANTICS = 1 << 17,  
    FLAG_EXECUTE = 1 << 18,           
  };

  
  
  
  
  
  
  
  enum Error {
    FILE_OK = 0,
    FILE_ERROR_FAILED = -1,
    FILE_ERROR_IN_USE = -2,
    FILE_ERROR_EXISTS = -3,
    FILE_ERROR_NOT_FOUND = -4,
    FILE_ERROR_ACCESS_DENIED = -5,
    FILE_ERROR_TOO_MANY_OPENED = -6,
    FILE_ERROR_NO_MEMORY = -7,
    FILE_ERROR_NO_SPACE = -8,
    FILE_ERROR_NOT_A_DIRECTORY = -9,
    FILE_ERROR_INVALID_OPERATION = -10,
    FILE_ERROR_SECURITY = -11,
    FILE_ERROR_ABORT = -12,
    FILE_ERROR_NOT_A_FILE = -13,
    FILE_ERROR_NOT_EMPTY = -14,
    FILE_ERROR_INVALID_URL = -15,
    FILE_ERROR_IO = -16,
    
    FILE_ERROR_MAX = -17
  };

  
  enum Whence {
    FROM_BEGIN   = 0,
    FROM_CURRENT = 1,
    FROM_END     = 2
  };

  
  
  
  
  
  struct BASE_EXPORT Info {
    Info();
    ~Info();

    
    int64 size;

    
    bool is_directory;

    
    bool is_symbolic_link;

    
    base::Time last_modified;

    
    base::Time last_accessed;

    
    base::Time creation_time;
  };

  File();

  
  
  File(const FilePath& name, uint32 flags);

  
  explicit File(PlatformFile platform_file);

  
  File(RValue other);

  ~File();

  
  File& operator=(RValue other);

  
  
  void CreateBaseFileUnsafe(const FilePath& name, uint32 flags);

  bool IsValid() const;

  
  
  
  bool created() const { return created_; }

  
  Error error() const { return error_; }

  PlatformFile GetPlatformFile() const { return file_; }
  PlatformFile TakePlatformFile();

  
  void Close();

  
  
  
  int64 Seek(Whence whence, int64 offset);

  
  
  
  
  
  
  int Read(int64 offset, char* data, int size);

  
  int ReadAtCurrentPos(char* data, int size);

  
  
  
  int ReadNoBestEffort(int64 offset, char* data, int size);

  
  int ReadAtCurrentPosNoBestEffort(char* data, int size);

  
  // data that was previously there. Returns the number of bytes written, or -1
  
  
  
  
  int Write(int64 offset, const char* data, int size);

  
  int WriteAtCurrentPos(const char* data, int size);

  
  // platforms. Returns the number of bytes written, or -1 on error.
  int WriteAtCurrentPosNoBestEffort(const char* data, int size);

  
  
  
  bool Truncate(int64 length);

  
  bool Flush();

  
  bool SetTimes(Time last_access_time, Time last_modified_time);

  
  bool GetInfo(Info* info);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  Error Lock();

  
  Error Unlock();

#if defined(OS_WIN)
  static Error OSErrorToFileError(DWORD last_error);
#elif defined(OS_POSIX)
  static Error OSErrorToFileError(int saved_errno);
#endif

 private:
  void SetPlatformFile(PlatformFile file);

#if defined(OS_WIN)
  win::ScopedHandle file_;
#elif defined(OS_POSIX)
  PlatformFile file_;
#endif

  Error error_;
  bool created_;
  bool async_;
};

}  

#endif  
