// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_VISITEDLINK_VISITEDLINK_MASTER_H_
#define CHROME_BROWSER_VISITEDLINK_VISITEDLINK_MASTER_H_
#pragma once

#if defined(OS_WIN)
#include <windows.h>
#endif
#include <set>
#include <vector>

#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/shared_memory.h"
#include "chrome/browser/history/history.h"
#include "chrome/common/visitedlink_common.h"

class GURL;
class Profile;

class VisitedLinkMaster : public VisitedLinkCommon {
 public:
  
  
  class Listener {
   public:
    virtual ~Listener() {}

    
    
    virtual void NewTable(base::SharedMemory*) = 0;

    
    
    virtual void Add(Fingerprint fingerprint) = 0;

    
    
    virtual void Reset() = 0;
  };

  
  VisitedLinkMaster(Listener* listener, Profile* profile);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  VisitedLinkMaster(Listener* listener,
                    HistoryService* history_service,
                    bool suppress_rebuild,
                    const FilePath& filename,
                    int32 default_table_size);
  virtual ~VisitedLinkMaster();

  
  
  
  bool Init();

  base::SharedMemory* shared_memory() { return shared_memory_; }

  
  void AddURL(const GURL& url);

  
  void AddURLs(const std::vector<GURL>& url);

  
  void DeleteURLs(const std::set<GURL>& urls);

  
  
  void DeleteAllURLs();

#if defined(UNIT_TEST) || !defined(NDEBUG) || defined(PERF_TEST)
  
  
  void DebugValidate();

  
  
  
  void set_rebuild_complete_task(Task* task) {
    DCHECK(!rebuild_complete_task_.get());
    rebuild_complete_task_.reset(task);
  }

  
  int32 GetUsedCount() const {
    return used_items_;
  }

  // Call to cause the entire database file to be re-written from scratch
  
  bool RewriteFile() {
    return WriteFullTable();
  }
#endif

 private:
  FRIEND_TEST_ALL_PREFIXES(VisitedLinkTest, Delete);
  FRIEND_TEST_ALL_PREFIXES(VisitedLinkTest, BigDelete);
  FRIEND_TEST_ALL_PREFIXES(VisitedLinkTest, BigImport);

  
  class TableBuilder;

  
  static const int32 kFileHeaderSignatureOffset;
  static const int32 kFileHeaderVersionOffset;
  static const int32 kFileHeaderLengthOffset;
  static const int32 kFileHeaderUsedOffset;
  static const int32 kFileHeaderSaltOffset;

  
  static const int32 kFileSignature;

  
  static const int32 kFileCurrentVersion;

  
  static const size_t kFileHeaderSize;

  
  static const unsigned kDefaultTableSize;

  
  
  
  static const size_t kBigDeleteThreshold;

  
  void InitMembers(Listener* listener, Profile* profile);

  
  
  
  Hash TryToAddURL(const GURL& url);

  
  

  
  
  bool WriteFullTable();

  
  
  bool InitFromFile();

  
  
  
  
  
  
  
  bool ReadFileHeader(FILE* hfile, int32* num_entries, int32* used_count,
                      uint8 salt[LINK_SALT_LENGTH]);

  
  bool GetDatabaseFileName(FilePath* filename);

  
  
  void WriteToFile(FILE* hfile, off_t offset, void* data, int32 data_size);

  
  
  void WriteUsedItemCountToFile();

  
  
  
  void WriteHashRangeToFile(Hash first_hash, Hash last_hash);

  
  
  bool ReadFromFile(FILE* hfile, off_t offset, void* data, size_t data_size);

  
  

  
  
  
  
  Hash AddFingerprint(Fingerprint fingerprint, bool send_notifications);

  
  
  
  
  void DeleteFingerprintsFromCurrentTable(
      const std::set<Fingerprint>& fingerprints);

  
  // is set, the changes will also be written to disk. Returns true if the
  
  bool DeleteFingerprint(Fingerprint fingerprint, bool update_file);

  
  
  
  
  
  bool InitFromScratch(bool suppress_rebuild);

  
  
  
  
  
  bool CreateURLTable(int32 num_entries, bool init_to_empty);

  
  
  
  
  
  
  
  
  
  bool BeginReplaceURLTable(int32 num_entries);

  
  void FreeURLTable();

  
  
  
  bool ResizeTableIfNecessary();

  
  
  void ResizeTable(int32 new_size);

  
  uint32 NewTableSizeForCount(int32 item_count) const;

  
  
  float ComputeTableLoad() const {
    return static_cast<float>(used_items_) / static_cast<float>(table_length_);
  }

  
  
  
  
  
  
  
  bool RebuildTableFromHistory();

  
  
  
  
  void OnTableRebuildComplete(bool success,
                              const std::vector<Fingerprint>& fingerprints);

  
  
  inline Hash IncrementHash(Hash hash) {
    if (hash >= table_length_ - 1)
      return 0;  
    return hash + 1;
  }
  inline Hash DecrementHash(Hash hash) {
    if (hash <= 0)
      return table_length_ - 1;  
    return hash - 1;
  }

  Listener* listener_;

#ifndef NDEBUG
  
  
  
  
  
  bool posted_asynchronous_operation_;
#endif

  
  
  Profile* profile_;

  
  
  
  
  scoped_refptr<TableBuilder> table_builder_;

  
  std::set<Fingerprint> added_since_rebuild_;
  std::set<Fingerprint> deleted_since_rebuild_;

  
  
  
  

  
  // rebuilding and haven't written a new version yet. Writing to the file may
  
  FILE* file_;

  
  base::SharedMemory *shared_memory_;

  
  
  int32 shared_memory_serial_;

  
  int32 used_items_;

  
  
  
  
  
  
  

  
  FilePath database_name_override_;

  
  int32 table_size_override_;

  
  
  HistoryService* history_service_override_;

  
  
  scoped_ptr<Task> rebuild_complete_task_;

  
  
  
  bool suppress_rebuild_;

  DISALLOW_COPY_AND_ASSIGN(VisitedLinkMaster);
};


#if defined(UNIT_TEST) || defined(PERF_TEST) || !defined(NDEBUG)
inline void VisitedLinkMaster::DebugValidate() {
  int32 used_count = 0;
  for (int32 i = 0; i < table_length_; i++) {
    if (hash_table_[i])
      used_count++;
  }
  DCHECK_EQ(used_count, used_items_);
}
#endif

#endif  
