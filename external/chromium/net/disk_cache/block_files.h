// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_DISK_CACHE_BLOCK_FILES_H__
#define NET_DISK_CACHE_BLOCK_FILES_H__
#pragma once

#include <vector>

#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "net/disk_cache/addr.h"
#include "net/disk_cache/mapped_file.h"

namespace base {
class ThreadChecker;
}

namespace disk_cache {

class BlockFiles {
 public:
  explicit BlockFiles(const FilePath& path);
  ~BlockFiles();

  
  
  bool Init(bool create_files);

  
  MappedFile* GetFile(Addr address);

  
  
  
  bool CreateBlock(FileType block_type, int block_count, Addr* block_address);

  
  
  
  void DeleteBlock(Addr address, bool deep);

  
  
  void CloseFiles();

  
  void ReportStats();

  
  
  bool IsValid(Addr address);

 private:
  
  bool CreateBlockFile(int index, FileType file_type, bool force);
  bool OpenBlockFile(int index);

  
  bool GrowBlockFile(MappedFile* file, BlockFileHeader* header);

  
  MappedFile* FileForNewBlock(FileType block_type, int block_count);

  
  MappedFile* NextFile(const MappedFile* file);

  
  int CreateNextBlockFile(FileType block_type);

  
  void RemoveEmptyFile(FileType block_type);

  
  bool FixBlockFileHeader(MappedFile* file);

  
  void GetFileStats(int index, int* used_count, int* load);

  
  FilePath Name(int index);

  bool init_;
  char* zero_buffer_;  
  FilePath path_;  
  std::vector<MappedFile*> block_files_;  
  scoped_ptr<base::ThreadChecker> thread_checker_;

  FRIEND_TEST_ALL_PREFIXES(DiskCacheTest, BlockFiles_ZeroSizeFile);
  FRIEND_TEST_ALL_PREFIXES(DiskCacheTest, BlockFiles_TruncatedFile);
  FRIEND_TEST_ALL_PREFIXES(DiskCacheTest, BlockFiles_InvalidFile);
  FRIEND_TEST_ALL_PREFIXES(DiskCacheTest, BlockFiles_Stats);

  DISALLOW_COPY_AND_ASSIGN(BlockFiles);
};

}  

#endif  
