// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_FILES_FILE_ENUMERATOR_H_
#define BASE_FILES_FILE_ENUMERATOR_H_

#include <stack>
#include <vector>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/time/time.h"
#include "build/build_config.h"

#if defined(OS_WIN)
#include <windows.h>
#elif defined(OS_POSIX)
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace base {

class BASE_EXPORT FileEnumerator {
 public:
  
  class BASE_EXPORT FileInfo {
   public:
    FileInfo();
    ~FileInfo();

    bool IsDirectory() const;

    
    
    
    FilePath GetName() const;

    int64 GetSize() const;
    Time GetLastModifiedTime() const;

#if defined(OS_WIN)
    
    
    
    const WIN32_FIND_DATA& find_data() const { return find_data_; }
#elif defined(OS_POSIX)
    const struct stat& stat() const { return stat_; }
#endif

   private:
    friend class FileEnumerator;

#if defined(OS_WIN)
    WIN32_FIND_DATA find_data_;
#elif defined(OS_POSIX)
    struct stat stat_;
    FilePath filename_;
#endif
  };

  enum FileType {
    FILES                 = 1 << 0,
    DIRECTORIES           = 1 << 1,
    INCLUDE_DOT_DOT       = 1 << 2,
#if defined(OS_POSIX)
    SHOW_SYM_LINKS        = 1 << 4,
#endif
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  FileEnumerator(const FilePath& root_path,
                 bool recursive,
                 int file_type);
  FileEnumerator(const FilePath& root_path,
                 bool recursive,
                 int file_type,
                 const FilePath::StringType& pattern);
  ~FileEnumerator();

  
  
  
  
  
  FilePath Next();

  
  FileInfo GetInfo() const;

 private:
  
  bool ShouldSkip(const FilePath& path);

#if defined(OS_WIN)
  
  bool has_find_data_;
  WIN32_FIND_DATA find_data_;
  HANDLE find_handle_;
#elif defined(OS_POSIX)

  
  static bool ReadDirectory(std::vector<FileInfo>* entries,
                            const FilePath& source, bool show_links);

  
  std::vector<FileInfo> directory_entries_;

  
  size_t current_directory_entry_;
#endif

  FilePath root_path_;
  bool recursive_;
  int file_type_;
  FilePath::StringType pattern_;  

  
  
  std::stack<FilePath> pending_paths_;

  DISALLOW_COPY_AND_ASSIGN(FileEnumerator);
};

}  

#endif  
