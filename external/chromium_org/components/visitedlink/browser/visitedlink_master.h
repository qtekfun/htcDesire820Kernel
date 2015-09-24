// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_VISITEDLINK_BROWSER_VISITEDLINK_MASTER_H_
#define COMPONENTS_VISITEDLINK_BROWSER_VISITEDLINK_MASTER_H_

#if defined(OS_WIN)
#include <windows.h>
#endif
#include <set>
#include <vector>

#include "base/callback.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/shared_memory.h"
#include "base/threading/sequenced_worker_pool.h"
#include "components/visitedlink/common/visitedlink_common.h"

#if defined(UNIT_TEST) || defined(PERF_TEST) || !defined(NDEBUG)
#include "base/logging.h"
#endif

class GURL;

namespace content {
class BrowserContext;
}

namespace visitedlink {

class VisitedLinkDelegate;

class VisitedLinkMaster : public VisitedLinkCommon {
 public:
  
  
  class Listener {
   public:
    virtual ~Listener() {}

    
    
    virtual void NewTable(base::SharedMemory*) = 0;

    
    
    virtual void Add(Fingerprint fingerprint) = 0;

    
    
    virtual void Reset() = 0;
  };

  VisitedLinkMaster(content::BrowserContext* browser_context,
                    VisitedLinkDelegate* delegate,
                    bool persist_to_disk);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  VisitedLinkMaster(Listener* listener,
                    VisitedLinkDelegate* delegate,
                    bool persist_to_disk,
                    bool suppress_rebuild,
                    const base::FilePath& filename,
                    int32 default_table_size);
  virtual ~VisitedLinkMaster();

  
  
  
  bool Init();

  base::SharedMemory* shared_memory() { return shared_memory_; }

  
  void AddURL(const GURL& url);

  
  void AddURLs(const std::vector<GURL>& url);

  
  class URLIterator {
   public:
    
    
    
    virtual const GURL& NextURL() = 0;

    
    virtual bool HasNextURL() const = 0;

   protected:
    virtual ~URLIterator() {}
  };

  
  void DeleteURLs(URLIterator* iterator);

  
  
  void DeleteAllURLs();

  
  VisitedLinkDelegate* GetDelegate();

#if defined(UNIT_TEST) || !defined(NDEBUG) || defined(PERF_TEST)
  
  
  void DebugValidate();

  
  
  
  void set_rebuild_complete_task(const base::Closure& task) {
    DCHECK(rebuild_complete_task_.is_null());
    rebuild_complete_task_ = task;
  }

  
  int32 GetUsedCount() const {
    return used_items_;
  }

  
  VisitedLinkMaster::Listener* GetListener() const {
    return listener_.get();
  }

  // Call to cause the entire database file to be re-written from scratch
  
  void RewriteFile() {
    WriteFullTable();
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

  
  void InitMembers();

  
  
  
  Hash TryToAddURL(const GURL& url);

  
  
  

  
  void PostIOTask(const tracked_objects::Location& from_here,
                  const base::Closure& task);

  
  
  void WriteFullTable();

  
  
  bool InitFromFile();

  
  
  
  
  
  
  
  bool ReadFileHeader(FILE* hfile, int32* num_entries, int32* used_count,
                      uint8 salt[LINK_SALT_LENGTH]);

  
  bool GetDatabaseFileName(base::FilePath* filename);

  
  
  void WriteToFile(FILE** hfile, off_t offset, void* data, int32 data_size);

  
  
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

  
  
  
  
  
  
  
  bool RebuildTableFromDelegate();

  
  
  
  
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

#ifndef NDEBUG
  
  
  
  
  
  bool posted_asynchronous_operation_;
#endif

  
  
  content::BrowserContext* browser_context_;

  
  
  VisitedLinkDelegate* delegate_;

  
  scoped_ptr<Listener> listener_;

  
  base::SequencedWorkerPool::SequenceToken sequence_token_;

  
  
  
  
  scoped_refptr<TableBuilder> table_builder_;

  
  std::set<Fingerprint> added_since_rebuild_;
  std::set<Fingerprint> deleted_since_rebuild_;

  
  
  
  

  
  // rebuilding and haven't written a new version yet or if |persist_to_disk_|
  
  
  
  
  
  
  
  FILE** file_;

  
  
  bool persist_to_disk_;

  
  base::SharedMemory *shared_memory_;

  
  
  int32 shared_memory_serial_;

  
  int32 used_items_;

  
  
  
  
  
  
  

  
  base::FilePath database_name_override_;

  
  int32 table_size_override_;

  
  
  base::Closure rebuild_complete_task_;

  
  
  
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

}  

#endif  
