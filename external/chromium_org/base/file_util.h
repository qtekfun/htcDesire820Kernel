// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_FILE_UTIL_H_
#define BASE_FILE_UTIL_H_

#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#elif defined(OS_POSIX)
#include <sys/stat.h>
#include <unistd.h>
#endif

#include <stdio.h>

#include <set>
#include <string>
#include <vector>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/platform_file.h"
#include "base/strings/string16.h"

#if defined(OS_POSIX)
#include "base/file_descriptor_posix.h"
#include "base/logging.h"
#include "base/posix/eintr_wrapper.h"
#endif

namespace base {

class Time;


BASE_EXPORT FilePath MakeAbsoluteFilePath(const FilePath& input);

BASE_EXPORT int64 ComputeDirectorySize(const FilePath& root_path);

BASE_EXPORT bool DeleteFile(const FilePath& path, bool recursive);

#if defined(OS_WIN)
BASE_EXPORT bool DeleteFileAfterReboot(const FilePath& path);
#endif

BASE_EXPORT bool Move(const FilePath& from_path, const FilePath& to_path);

BASE_EXPORT bool ReplaceFile(const FilePath& from_path,
                             const FilePath& to_path,
                             PlatformFileError* error);

BASE_EXPORT bool CopyFile(const FilePath& from_path, const FilePath& to_path);

BASE_EXPORT bool CopyDirectory(const FilePath& from_path,
                               const FilePath& to_path,
                               bool recursive);

BASE_EXPORT bool PathExists(const FilePath& path);

BASE_EXPORT bool PathIsWritable(const FilePath& path);

BASE_EXPORT bool DirectoryExists(const FilePath& path);

BASE_EXPORT bool ContentsEqual(const FilePath& filename1,
                               const FilePath& filename2);

BASE_EXPORT bool TextContentsEqual(const FilePath& filename1,
                                   const FilePath& filename2);

BASE_EXPORT bool ReadFileToString(const FilePath& path, std::string* contents);

#if defined(OS_POSIX)

BASE_EXPORT bool ReadFromFD(int fd, char* buffer, size_t bytes);

BASE_EXPORT bool CreateSymbolicLink(const FilePath& target,
                                    const FilePath& symlink);

BASE_EXPORT bool ReadSymbolicLink(const FilePath& symlink, FilePath* target);

enum FilePermissionBits {
  FILE_PERMISSION_MASK              = S_IRWXU | S_IRWXG | S_IRWXO,
  FILE_PERMISSION_USER_MASK         = S_IRWXU,
  FILE_PERMISSION_GROUP_MASK        = S_IRWXG,
  FILE_PERMISSION_OTHERS_MASK       = S_IRWXO,

  FILE_PERMISSION_READ_BY_USER      = S_IRUSR,
  FILE_PERMISSION_WRITE_BY_USER     = S_IWUSR,
  FILE_PERMISSION_EXECUTE_BY_USER   = S_IXUSR,
  FILE_PERMISSION_READ_BY_GROUP     = S_IRGRP,
  FILE_PERMISSION_WRITE_BY_GROUP    = S_IWGRP,
  FILE_PERMISSION_EXECUTE_BY_GROUP  = S_IXGRP,
  FILE_PERMISSION_READ_BY_OTHERS    = S_IROTH,
  FILE_PERMISSION_WRITE_BY_OTHERS   = S_IWOTH,
  FILE_PERMISSION_EXECUTE_BY_OTHERS = S_IXOTH,
};

BASE_EXPORT bool GetPosixFilePermissions(const FilePath& path, int* mode);
BASE_EXPORT bool SetPosixFilePermissions(const FilePath& path, int mode);

#endif  

BASE_EXPORT bool IsDirectoryEmpty(const FilePath& dir_path);

BASE_EXPORT bool GetTempDir(FilePath* path);

BASE_EXPORT bool GetShmemTempDir(bool executable, FilePath* path);

#if defined(OS_POSIX)
BASE_EXPORT FilePath GetHomeDir();
#endif  

BASE_EXPORT bool CreateTemporaryFile(FilePath* path);

BASE_EXPORT bool CreateTemporaryFileInDir(const FilePath& dir,
                                          FilePath* temp_file);

BASE_EXPORT FILE* CreateAndOpenTemporaryFile(FilePath* path);

BASE_EXPORT FILE* CreateAndOpenTemporaryShmemFile(FilePath* path,
                                                  bool executable);

BASE_EXPORT FILE* CreateAndOpenTemporaryFileInDir(const FilePath& dir,
                                                  FilePath* path);

BASE_EXPORT bool CreateNewTempDirectory(const FilePath::StringType& prefix,
                                        FilePath* new_temp_path);

BASE_EXPORT bool CreateTemporaryDirInDir(const FilePath& base_dir,
                                         const FilePath::StringType& prefix,
                                         FilePath* new_dir);

BASE_EXPORT bool CreateDirectoryAndGetError(const FilePath& full_path,
                                            PlatformFileError* error);

BASE_EXPORT bool CreateDirectory(const FilePath& full_path);

BASE_EXPORT bool GetFileSize(const FilePath& file_path, int64* file_size);

BASE_EXPORT bool NormalizeFilePath(const FilePath& path, FilePath* real_path);

#if defined(OS_WIN)

BASE_EXPORT bool DevicePathToDriveLetterPath(const FilePath& device_path,
                                             FilePath* drive_letter_path);

BASE_EXPORT bool NormalizeToNativeFilePath(const FilePath& path,
                                           FilePath* nt_path);
#endif

BASE_EXPORT bool IsLink(const FilePath& file_path);

BASE_EXPORT bool GetFileInfo(const FilePath& file_path, PlatformFileInfo* info);

BASE_EXPORT bool TouchFile(const FilePath& path,
                           const Time& last_accessed,
                           const Time& last_modified);

BASE_EXPORT FILE* OpenFile(const FilePath& filename, const char* mode);

BASE_EXPORT bool CloseFile(FILE* file);

BASE_EXPORT bool TruncateFile(FILE* file);

BASE_EXPORT int ReadFile(const base::FilePath& filename, char* data, int size);

}  


namespace file_util {

// previously there.  Returns the number of bytes written, or -1 on error.
BASE_EXPORT int WriteFile(const base::FilePath& filename, const char* data,
                          int size);
#if defined(OS_POSIX)
BASE_EXPORT int WriteFileDescriptor(const int fd, const char* data, int size);
#endif
// Append the given buffer into the file. Returns the number of bytes written,
BASE_EXPORT int AppendToFile(const base::FilePath& filename,
                             const char* data, int size);

BASE_EXPORT bool GetCurrentDirectory(base::FilePath* path);

BASE_EXPORT bool SetCurrentDirectory(const base::FilePath& path);

BASE_EXPORT int GetUniquePathNumber(const base::FilePath& path,
                                    const base::FilePath::StringType& suffix);

#if defined(OS_POSIX)
BASE_EXPORT base::FilePath MakeUniqueDirectory(const base::FilePath& path);
#endif

#if defined(OS_POSIX)
BASE_EXPORT bool VerifyPathControlledByUser(const base::FilePath& base,
                                            const base::FilePath& path,
                                            uid_t owner_uid,
                                            const std::set<gid_t>& group_gids);
#endif  

#if defined(OS_MACOSX) && !defined(OS_IOS)
BASE_EXPORT bool VerifyPathControlledByAdmin(const base::FilePath& path);
#endif  

BASE_EXPORT int GetMaximumPathComponentLength(const base::FilePath& path);

class ScopedFILEClose {
 public:
  inline void operator()(FILE* x) const {
    if (x) {
      fclose(x);
    }
  }
};

typedef scoped_ptr_malloc<FILE, ScopedFILEClose> ScopedFILE;

#if defined(OS_POSIX)
class ScopedFDClose {
 public:
  inline void operator()(int* x) const {
    if (x && *x >= 0) {
      if (IGNORE_EINTR(close(*x)) < 0)
        DPLOG(ERROR) << "close";
    }
  }
};

typedef scoped_ptr_malloc<int, ScopedFDClose> ScopedFD;
#endif  

#if defined(OS_LINUX)
enum FileSystemType {
  FILE_SYSTEM_UNKNOWN,  
  FILE_SYSTEM_0,        
  FILE_SYSTEM_ORDINARY,       
  FILE_SYSTEM_NFS,
  FILE_SYSTEM_SMB,
  FILE_SYSTEM_CODA,
  FILE_SYSTEM_MEMORY,         
  FILE_SYSTEM_CGROUP,         
  FILE_SYSTEM_OTHER,          
  FILE_SYSTEM_TYPE_COUNT
};

BASE_EXPORT bool GetFileSystemType(const base::FilePath& path,
                                   FileSystemType* type);
#endif

}  


namespace base {
namespace internal {

BASE_EXPORT bool MoveUnsafe(const FilePath& from_path,
                            const FilePath& to_path);

BASE_EXPORT bool CopyFileUnsafe(const FilePath& from_path,
                                const FilePath& to_path);

#if defined(OS_WIN)
BASE_EXPORT bool CopyAndDeleteDirectory(const FilePath& from_path,
                                        const FilePath& to_path);
#endif  

}  
}  

#endif  
