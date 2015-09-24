// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_SIMPLE_SIMPLE_INDEX_FILE_H_
#define NET_DISK_CACHE_SIMPLE_SIMPLE_INDEX_FILE_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/pickle.h"
#include "base/port.h"
#include "net/base/cache_type.h"
#include "net/base/net_export.h"
#include "net/disk_cache/simple/simple_index.h"

namespace base {
class SingleThreadTaskRunner;
class TaskRunner;
}

namespace disk_cache {

const uint64 kSimpleIndexMagicNumber = GG_UINT64_C(0x656e74657220796f);

struct NET_EXPORT_PRIVATE SimpleIndexLoadResult {
  SimpleIndexLoadResult();
  ~SimpleIndexLoadResult();
  void Reset();

  bool did_load;
  SimpleIndex::EntrySet entries;
  bool flush_required;
};

class NET_EXPORT_PRIVATE SimpleIndexFile {
 public:
  class NET_EXPORT_PRIVATE IndexMetadata {
   public:
    IndexMetadata();
    IndexMetadata(uint64 number_of_entries, uint64 cache_size);

    void Serialize(Pickle* pickle) const;
    bool Deserialize(PickleIterator* it);

    bool CheckIndexMetadata();

    uint64 GetNumberOfEntries() { return number_of_entries_; }

   private:
    FRIEND_TEST_ALL_PREFIXES(IndexMetadataTest, Basics);
    FRIEND_TEST_ALL_PREFIXES(IndexMetadataTest, Serialize);

    uint64 magic_number_;
    uint32 version_;
    uint64 number_of_entries_;
    uint64 cache_size_;  
  };

  SimpleIndexFile(base::SingleThreadTaskRunner* cache_thread,
                  base::TaskRunner* worker_pool,
                  net::CacheType cache_type,
                  const base::FilePath& cache_directory);
  virtual ~SimpleIndexFile();

  
  virtual void LoadIndexEntries(base::Time cache_last_modified,
                                const base::Closure& callback,
                                SimpleIndexLoadResult* out_result);

  
  virtual void WriteToDisk(const SimpleIndex::EntrySet& entry_set,
                           uint64 cache_size,
                           const base::TimeTicks& start,
                           bool app_on_background);

 private:
  friend class WrappedSimpleIndexFile;

  
  typedef base::Callback<void (const base::FilePath&)> EntryFileCallback;

  
  
  static const int kExtraSizeForMerge = 512;

  
  static void SyncLoadIndexEntries(net::CacheType cache_type,
                                   base::Time cache_last_modified,
                                   const base::FilePath& cache_directory,
                                   const base::FilePath& index_file_path,
                                   SimpleIndexLoadResult* out_result);

  
  static void SyncLoadFromDisk(const base::FilePath& index_filename,
                               base::Time* out_last_cache_seen_by_index,
                               SimpleIndexLoadResult* out_result);

  
  // data to be written to a file. Note: the pickle is not in a consistent state
  
  
  static scoped_ptr<Pickle> Serialize(
      const SimpleIndexFile::IndexMetadata& index_metadata,
      const SimpleIndex::EntrySet& entries);

  
  
  
  
  static bool SerializeFinalData(base::Time cache_modified, Pickle* pickle);

  
  
  static void Deserialize(const char* data, int data_len,
                          base::Time* out_cache_last_modified,
                          SimpleIndexLoadResult* out_result);

  
  
  
  
  
  static bool TraverseCacheDirectory(
      const base::FilePath& cache_path,
      const EntryFileCallback& entry_file_callback);

  
  static void SyncWriteToDisk(net::CacheType cache_type,
                              const base::FilePath& cache_directory,
                              const base::FilePath& index_filename,
                              const base::FilePath& temp_index_filename,
                              scoped_ptr<Pickle> pickle,
                              const base::TimeTicks& start_time,
                              bool app_on_background);

  
  
  static void SyncRestoreFromDisk(const base::FilePath& cache_directory,
                                  const base::FilePath& index_file_path,
                                  SimpleIndexLoadResult* out_result);

  
  
  
  
  static bool LegacyIsIndexFileStale(base::Time cache_last_modified,
                                     const base::FilePath& index_file_path);

  struct PickleHeader : public Pickle::Header {
    uint32 crc;
  };

  const scoped_refptr<base::SingleThreadTaskRunner> cache_thread_;
  const scoped_refptr<base::TaskRunner> worker_pool_;
  const net::CacheType cache_type_;
  const base::FilePath cache_directory_;
  const base::FilePath index_file_;
  const base::FilePath temp_index_file_;

  static const char kIndexDirectory[];
  static const char kIndexFileName[];
  static const char kTempIndexFileName[];

  DISALLOW_COPY_AND_ASSIGN(SimpleIndexFile);
};


}  

#endif  
