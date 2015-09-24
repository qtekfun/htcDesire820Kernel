// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_FILE_CACHE_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_FILE_CACHE_H_

#include <set>
#include <string>

#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/chromeos/drive/file_errors.h"
#include "chrome/browser/chromeos/drive/resource_metadata_storage.h"

namespace base {
class SequencedTaskRunner;
}  

namespace drive {

class FileCacheEntry;

namespace internal {

class FreeDiskSpaceGetterInterface {
 public:
  virtual ~FreeDiskSpaceGetterInterface() {}
  virtual int64 AmountOfFreeDiskSpace() = 0;
};

class FileCache {
 public:
  
  enum FileOperationType {
    FILE_OPERATION_MOVE = 0,
    FILE_OPERATION_COPY,
  };

  typedef ResourceMetadataStorage::CacheEntryIterator Iterator;

  
  
  
  
  
  
  
  
  
  
  FileCache(ResourceMetadataStorage* storage,
            const base::FilePath& cache_file_directory,
            base::SequencedTaskRunner* blocking_task_runner,
            FreeDiskSpaceGetterInterface* free_disk_space_getter);

  
  
  
  
  bool IsUnderFileCacheDirectory(const base::FilePath& path) const;

  
  
  bool GetCacheEntry(const std::string& id, FileCacheEntry* entry);

  
  scoped_ptr<Iterator> GetIterator();

  
  
  
  
  bool FreeDiskSpaceIfNeededFor(int64 num_bytes);

  
  
  
  FileError GetFile(const std::string& id, base::FilePath* cache_file_path);

  
  
  FileError Store(const std::string& id,
                  const std::string& md5,
                  const base::FilePath& source_path,
                  FileOperationType file_operation_type);

  
  FileError Pin(const std::string& id);

  
  FileError Unpin(const std::string& id);

  
  FileError MarkAsMounted(const std::string& id,
                          base::FilePath* cache_file_path);

  
  FileError MarkAsUnmounted(const base::FilePath& file_path);

  
  FileError MarkDirty(const std::string& id);

  
  FileError ClearDirty(const std::string& id, const std::string& md5);

  
  FileError Remove(const std::string& id);

  
  bool ClearAll();

  
  bool Initialize();

  
  
  
  void Destroy();

  
  
  
  
  bool RecoverFilesFromCacheDirectory(
      const base::FilePath& dest_directory,
      const ResourceMetadataStorage::RecoveredCacheInfoMap&
          recovered_cache_info);

 private:
  friend class FileCacheTest;
  friend class FileCacheTestOnUIThread;

  ~FileCache();

  
  
  
  base::FilePath GetCacheFilePath(const std::string& id) const;

  
  
  void AssertOnSequencedWorkerPool();

  
  void DestroyOnBlockingPool();

  
  
  bool HasEnoughSpaceFor(int64 num_bytes, const base::FilePath& path);

  
  
  bool RenameCacheFilesToNewFormat();

  const base::FilePath cache_file_directory_;

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;

  ResourceMetadataStorage* storage_;

  FreeDiskSpaceGetterInterface* free_disk_space_getter_;  

  
  std::set<std::string> mounted_files_;

  
  
  base::WeakPtrFactory<FileCache> weak_ptr_factory_;
  DISALLOW_COPY_AND_ASSIGN(FileCache);
};

}  
}  

#endif  
