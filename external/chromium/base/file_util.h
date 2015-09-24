// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_FILE_UTIL_H_
#define BASE_FILE_UTIL_H_
#pragma once

#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#if defined(UNIT_TEST)
#include <aclapi.h>
#endif
#elif defined(OS_POSIX)
#include <sys/stat.h>
#endif

#include <stdio.h>

#include <stack>
#include <string>
#include <vector>

#include "base/base_api.h"
#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/platform_file.h"
#include "base/string16.h"

#if defined(OS_POSIX)
#include "base/eintr_wrapper.h"
#include "base/file_descriptor_posix.h"
#include "base/logging.h"
#endif

namespace base {
class Time;
}

namespace file_util {


BASE_API bool EndsWithSeparator(const FilePath& path);

BASE_API bool EnsureEndsWithSeparator(FilePath* path);

BASE_API bool AbsolutePath(FilePath* path);

BASE_API bool ContainsPath(const FilePath& parent, const FilePath& child);


BASE_API int CountFilesCreatedAfter(const FilePath& path,
                                    const base::Time& file_time);

BASE_API int64 ComputeDirectorySize(const FilePath& root_path);

BASE_API int64 ComputeFilesSize(const FilePath& directory,
                                const FilePath::StringType& pattern);

BASE_API bool Delete(const FilePath& path, bool recursive);

#if defined(OS_WIN)
BASE_API bool DeleteAfterReboot(const FilePath& path);
#endif

BASE_API bool Move(const FilePath& from_path, const FilePath& to_path);

BASE_API bool ReplaceFile(const FilePath& from_path, const FilePath& to_path);

BASE_API bool CopyFile(const FilePath& from_path, const FilePath& to_path);

BASE_API bool CopyDirectory(const FilePath& from_path, const FilePath& to_path,
                            bool recursive);

BASE_API bool PathExists(const FilePath& path);

BASE_API bool PathIsWritable(const FilePath& path);

BASE_API bool DirectoryExists(const FilePath& path);

#if defined(OS_WIN)
BASE_API bool GetFileCreationLocalTime(const std::wstring& filename,
                                       LPSYSTEMTIME creation_time);

BASE_API bool GetFileCreationLocalTimeFromHandle(HANDLE file_handle,
                                                 LPSYSTEMTIME creation_time);
#endif  

BASE_API bool ContentsEqual(const FilePath& filename1,
                            const FilePath& filename2);

BASE_API bool TextContentsEqual(const FilePath& filename1,
                                const FilePath& filename2);

BASE_API bool ReadFileToString(const FilePath& path, std::string* contents);

#if defined(OS_POSIX)
bool ReadFromFD(int fd, char* buffer, size_t bytes);

bool CreateSymbolicLink(const FilePath& target, const FilePath& symlink);

bool ReadSymbolicLink(const FilePath& symlink, FilePath* target);
#endif  

#if defined(OS_WIN)
BASE_API bool ResolveShortcut(FilePath* path);

BASE_API bool CreateShortcutLink(const wchar_t *source,
                                 const wchar_t *destination,
                                 const wchar_t *working_dir,
                                 const wchar_t *arguments,
                                 const wchar_t *description,
                                 const wchar_t *icon,
                                 int icon_index,
                                 const wchar_t* app_id);

BASE_API bool UpdateShortcutLink(const wchar_t *source,
                                 const wchar_t *destination,
                                 const wchar_t *working_dir,
                                 const wchar_t *arguments,
                                 const wchar_t *description,
                                 const wchar_t *icon,
                                 int icon_index,
                                 const wchar_t* app_id);

BASE_API bool TaskbarPinShortcutLink(const wchar_t* shortcut);

BASE_API bool TaskbarUnpinShortcutLink(const wchar_t* shortcut);

BASE_API bool CopyAndDeleteDirectory(const FilePath& from_path,
                                     const FilePath& to_path);
#endif  

BASE_API bool IsDirectoryEmpty(const FilePath& dir_path);

BASE_API bool GetTempDir(FilePath* path);
BASE_API bool GetShmemTempDir(FilePath* path);

BASE_API FilePath GetHomeDir();

BASE_API bool CreateTemporaryFile(FilePath* path);

BASE_API bool CreateTemporaryFileInDir(const FilePath& dir,
                                       FilePath* temp_file);

BASE_API FILE* CreateAndOpenTemporaryFile(FilePath* path);
BASE_API FILE* CreateAndOpenTemporaryShmemFile(FilePath* path);
BASE_API FILE* CreateAndOpenTemporaryFileInDir(const FilePath& dir,
                                               FilePath* path);

BASE_API bool CreateNewTempDirectory(const FilePath::StringType& prefix,
                                     FilePath* new_temp_path);

BASE_API bool CreateTemporaryDirInDir(const FilePath& base_dir,
                                      const FilePath::StringType& prefix,
                                      FilePath* new_dir);

BASE_API bool CreateDirectory(const FilePath& full_path);

BASE_API bool GetFileSize(const FilePath& file_path, int64* file_size);

BASE_API bool IsDot(const FilePath& path);

BASE_API bool IsDotDot(const FilePath& path);

BASE_API bool NormalizeFilePath(const FilePath& path, FilePath* real_path);

#if defined(OS_WIN)
BASE_API bool NormalizeToNativeFilePath(const FilePath& path,
                                        FilePath* nt_path);
#endif

BASE_API bool GetFileInfo(const FilePath& file_path,
                          base::PlatformFileInfo* info);

BASE_API bool TouchFile(const FilePath& path,
                        const base::Time& last_accessed,
                        const base::Time& last_modified);

BASE_API bool SetLastModifiedTime(const FilePath& path,
                                  const base::Time& last_modified);

#if defined(OS_POSIX)
bool GetInode(const FilePath& path, ino_t* inode);
#endif

BASE_API FILE* OpenFile(const FilePath& filename, const char* mode);

BASE_API bool CloseFile(FILE* file);

BASE_API bool TruncateFile(FILE* file);

BASE_API int ReadFile(const FilePath& filename, char* data, int size);

// previously there.  Returns the number of bytes written, or -1 on error.
BASE_API int WriteFile(const FilePath& filename, const char* data, int size);
#if defined(OS_POSIX)
int WriteFileDescriptor(const int fd, const char* data, int size);
#endif

BASE_API bool GetCurrentDirectory(FilePath* path);

BASE_API bool SetCurrentDirectory(const FilePath& path);

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
      if (HANDLE_EINTR(close(*x)) < 0)
        PLOG(ERROR) << "close";
    }
  }
};

typedef scoped_ptr_malloc<int, ScopedFDClose> ScopedFD;
#endif  

class BASE_API FileEnumerator {
 public:
#if defined(OS_WIN)
  typedef WIN32_FIND_DATA FindInfo;
#elif defined(OS_POSIX)
  typedef struct {
    struct stat stat;
    std::string filename;
  } FindInfo;
#endif

  enum FILE_TYPE {
    FILES                 = 1 << 0,
    DIRECTORIES           = 1 << 1,
    INCLUDE_DOT_DOT       = 1 << 2,
#if defined(OS_POSIX)
    SHOW_SYM_LINKS        = 1 << 4,
#endif
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  FileEnumerator(const FilePath& root_path,
                 bool recursive,
                 FileEnumerator::FILE_TYPE file_type);
  FileEnumerator(const FilePath& root_path,
                 bool recursive,
                 FileEnumerator::FILE_TYPE file_type,
                 const FilePath::StringType& pattern);
  ~FileEnumerator();

  
  FilePath Next();

  
  void GetFindInfo(FindInfo* info);

  
  static bool IsDirectory(const FindInfo& info);

  static FilePath GetFilename(const FindInfo& find_info);

 private:
  
  bool ShouldSkip(const FilePath& path);


#if defined(OS_WIN)
  
  bool has_find_data_;
  WIN32_FIND_DATA find_data_;
  HANDLE find_handle_;
#elif defined(OS_POSIX)
  struct DirectoryEntryInfo {
    FilePath filename;
    struct stat stat;
  };

  
  static bool ReadDirectory(std::vector<DirectoryEntryInfo>* entries,
                            const FilePath& source, bool show_links);

  
  std::vector<DirectoryEntryInfo> directory_entries_;

  
  size_t current_directory_entry_;
#endif

  FilePath root_path_;
  bool recursive_;
  FILE_TYPE file_type_;
  FilePath::StringType pattern_;  

  
  
  std::stack<FilePath> pending_paths_;

  DISALLOW_COPY_AND_ASSIGN(FileEnumerator);
};

class BASE_API MemoryMappedFile {
 public:
  
  MemoryMappedFile();
  ~MemoryMappedFile();

  
  
  
  
  
  bool Initialize(const FilePath& file_name);
  
  
  bool Initialize(base::PlatformFile file);

#if defined(OS_WIN)
  
  
  bool InitializeAsImageSection(const FilePath& file_name);
#endif  

  const uint8* data() const { return data_; }
  size_t length() const { return length_; }

  
  bool IsValid() const;

 private:
  
  bool MapFileToMemory(const FilePath& file_name);

  
  
  bool MapFileToMemoryInternal();

  
  void CloseHandles();

#if defined(OS_WIN)
  
  
  bool MapFileToMemoryInternalEx(int flags);

  HANDLE file_mapping_;
#endif
  base::PlatformFile file_;
  uint8* data_;
  size_t length_;

  DISALLOW_COPY_AND_ASSIGN(MemoryMappedFile);
};

BASE_API bool RenameFileAndResetSecurityDescriptor(
    const FilePath& source_file_path,
    const FilePath& target_file_path);

BASE_API bool HasFileBeenModifiedSince(
    const FileEnumerator::FindInfo& find_info,
    const base::Time& cutoff_time);

#ifdef UNIT_TEST

inline bool MakeFileUnreadable(const FilePath& path) {
#if defined(OS_POSIX)
  struct stat stat_buf;
  if (stat(path.value().c_str(), &stat_buf) != 0)
    return false;
  stat_buf.st_mode &= ~(S_IRUSR | S_IRGRP | S_IROTH);

  return chmod(path.value().c_str(), stat_buf.st_mode) == 0;

#elif defined(OS_WIN)
  PACL old_dacl;
  PSECURITY_DESCRIPTOR security_descriptor;
  if (GetNamedSecurityInfo(const_cast<wchar_t*>(path.value().c_str()),
                           SE_FILE_OBJECT,
                           DACL_SECURITY_INFORMATION, NULL, NULL, &old_dacl,
                           NULL, &security_descriptor) != ERROR_SUCCESS)
    return false;

  
  EXPLICIT_ACCESS change;
  change.grfAccessPermissions = GENERIC_READ;
  change.grfAccessMode = DENY_ACCESS;
  change.grfInheritance = 0;
  change.Trustee.pMultipleTrustee = NULL;
  change.Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
  change.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
  change.Trustee.TrusteeType = TRUSTEE_IS_USER;
  change.Trustee.ptstrName = L"CURRENT_USER";

  PACL new_dacl;
  if (SetEntriesInAcl(1, &change, old_dacl, &new_dacl) != ERROR_SUCCESS) {
    LocalFree(security_descriptor);
    return false;
  }

  DWORD rc = SetNamedSecurityInfo(const_cast<wchar_t*>(path.value().c_str()),
                                  SE_FILE_OBJECT, DACL_SECURITY_INFORMATION,
                                  NULL, NULL, new_dacl, NULL);
  LocalFree(security_descriptor);
  LocalFree(new_dacl);

  return rc == ERROR_SUCCESS;
#else
  NOTIMPLEMENTED();
  return false;
#endif
}

#endif  

#if defined(OS_WIN)
  
  
  
  
  
bool BASE_API PreReadImage(const wchar_t* file_path, size_t size_to_read,
                           size_t step_size);
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

bool GetFileSystemType(const FilePath& path, FileSystemType* type);
#endif

}  

#include "base/file_util_deprecated.h"

#endif  
