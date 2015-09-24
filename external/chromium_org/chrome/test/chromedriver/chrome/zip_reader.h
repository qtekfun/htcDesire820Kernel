// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_CHROMEDRIVER_CHROME_ZIP_READER_H_
#define CHROME_TEST_CHROMEDRIVER_CHROME_ZIP_READER_H_

#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/platform_file.h"
#include "base/time/time.h"

#if defined(USE_SYSTEM_MINIZIP)
#include <minizip/unzip.h>
#else
#include "third_party/zlib/contrib/minizip/unzip.h"
#endif

namespace zip {

class ZipReader {
 public:
  
  
  class EntryInfo {
   public:
    EntryInfo(const std::string& filename_in_zip,
              const unz_file_info& raw_file_info);

    
    
    const base::FilePath& file_path() const { return file_path_; }

    
    
    int64 original_size() const { return original_size_; }

    
    base::Time last_modified() const { return last_modified_; }

    
    bool is_directory() const { return is_directory_; }

    
    
    bool is_unsafe() const { return is_unsafe_; }

   private:
    const base::FilePath file_path_;
    int64 original_size_;
    base::Time last_modified_;
    bool is_directory_;
    bool is_unsafe_;
    DISALLOW_COPY_AND_ASSIGN(EntryInfo);
  };

  ZipReader();
  ~ZipReader();

  
  
  bool Open(const base::FilePath& zip_file_path);

  
  
  bool OpenFromPlatformFile(base::PlatformFile zip_fd);

  
  
  
  bool OpenFromString(const std::string& data);

  
  
  void Close();

  
  
  
  
  
  
  
  bool HasMore();

  
  bool AdvanceToNextEntry();

  
  
  
  
  
  
  
  bool OpenCurrentEntryInZip();

  
  
  
  bool LocateAndOpenEntry(const base::FilePath& path_in_zip);

  
  
  
  
  
  
  bool ExtractCurrentEntryToFilePath(const base::FilePath& output_file_path);

  
  
  
  
  
  
  
  
  bool ExtractCurrentEntryIntoDirectory(
      const base::FilePath& output_directory_path);

#if defined(OS_POSIX)
  
  
  bool ExtractCurrentEntryToFd(int fd);
#endif

  
  
  EntryInfo* current_entry_info() const {
    return current_entry_info_.get();
  }

  
  
  int num_entries() const { return num_entries_; }

 private:
  
  bool OpenInternal();

  
  void Reset();

  unzFile zip_file_;
  int num_entries_;
  bool reached_end_;
  scoped_ptr<EntryInfo> current_entry_info_;

  DISALLOW_COPY_AND_ASSIGN(ZipReader);
};

}  

#endif  
