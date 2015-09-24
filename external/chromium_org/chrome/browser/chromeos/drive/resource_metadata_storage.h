// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_RESOURCE_METADATA_STORAGE_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_RESOURCE_METADATA_STORAGE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/chromeos/drive/drive.pb.h"
#include "chrome/browser/drive/drive_service_interface.h"

namespace base {
class SequencedTaskRunner;
}

namespace leveldb {
class DB;
class Iterator;
}

namespace drive {

class ResourceEntry;
class ResourceMetadataHeader;

namespace internal {

class ResourceMetadataStorage {
 public:
  
  
  static const int kDBVersion = 12;

  
  class Iterator {
   public:
    explicit Iterator(scoped_ptr<leveldb::Iterator> it);
    ~Iterator();

    
    
    bool IsAtEnd() const;

    
    std::string GetID() const;

    
    const ResourceEntry& GetValue() const;

    
    bool GetCacheEntry(FileCacheEntry* cache_entry);

    
    void Advance();

    
    bool HasError() const;

   private:
    ResourceEntry entry_;
    scoped_ptr<leveldb::Iterator> it_;

    DISALLOW_COPY_AND_ASSIGN(Iterator);
  };

  
  class CacheEntryIterator {
   public:
    explicit CacheEntryIterator(scoped_ptr<leveldb::Iterator> it);
    ~CacheEntryIterator();

    
    
    
    bool IsAtEnd() const;

    
    const std::string& GetID() const;

    
    const FileCacheEntry& GetValue() const;

    
    void Advance();

    
    bool HasError() const;

   private:
    
    void AdvanceInternal();

    scoped_ptr<leveldb::Iterator> it_;
    std::string id_;
    FileCacheEntry entry_;

    DISALLOW_COPY_AND_ASSIGN(CacheEntryIterator);
  };

  
  struct RecoveredCacheInfo {
    RecoveredCacheInfo();
    ~RecoveredCacheInfo();

    bool is_dirty;
    std::string md5;
    std::string title;
  };
  typedef std::map<std::string, RecoveredCacheInfo> RecoveredCacheInfoMap;

  
  static bool UpgradeOldDB(const base::FilePath& directory_path,
                           const ResourceIdCanonicalizer& id_canonicalizer);

  ResourceMetadataStorage(const base::FilePath& directory_path,
                          base::SequencedTaskRunner* blocking_task_runner);

  const base::FilePath& directory_path() const { return directory_path_; }

  
  
  bool cache_file_scan_is_needed() const { return cache_file_scan_is_needed_; }

  
  void Destroy();

  
  bool Initialize();

  
  void RecoverCacheInfoFromTrashedResourceMap(RecoveredCacheInfoMap* out_info);

  
  bool SetLargestChangestamp(int64 largest_changestamp);

  
  int64 GetLargestChangestamp();

  
  bool PutEntry(const ResourceEntry& entry);

  
  bool GetEntry(const std::string& id, ResourceEntry* out_entry);

  
  bool RemoveEntry(const std::string& id);

  
  scoped_ptr<Iterator> GetIterator();

  
  std::string GetChild(const std::string& parent_id,
                       const std::string& child_name);

  
  void GetChildren(const std::string& parent_id,
                   std::vector<std::string>* children);

  
  bool PutCacheEntry(const std::string& id, const FileCacheEntry& entry);

  
  bool GetCacheEntry(const std::string& id, FileCacheEntry* out_entry);

  
  bool RemoveCacheEntry(const std::string& id);

  
  scoped_ptr<CacheEntryIterator> GetCacheEntryIterator();

  
  bool GetIdByResourceId(const std::string& resource_id, std::string* out_id);

 private:
  friend class ResourceMetadataStorageTest;

  
  ~ResourceMetadataStorage();

  
  void DestroyOnBlockingPool();

  
  static std::string GetChildEntryKey(const std::string& parent_id,
                                      const std::string& child_name);

  
  bool PutHeader(const ResourceMetadataHeader& header);

  
  bool GetHeader(ResourceMetadataHeader* out_header);

  
  bool CheckValidity();

  
  base::FilePath directory_path_;

  bool cache_file_scan_is_needed_;

  
  scoped_ptr<leveldb::DB> resource_map_;

  scoped_refptr<base::SequencedTaskRunner> blocking_task_runner_;

  DISALLOW_COPY_AND_ASSIGN(ResourceMetadataStorage);
};

}  
}  

#endif  
