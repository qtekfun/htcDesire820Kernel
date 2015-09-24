// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_STORE_FILE_H_
#define CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_STORE_FILE_H_

#include <set>
#include <vector>

#include "chrome/browser/safe_browsing/safe_browsing_store.h"

#include "base/callback.h"
#include "base/file_util.h"



class SafeBrowsingStoreFile : public SafeBrowsingStore {
 public:
  SafeBrowsingStoreFile();
  virtual ~SafeBrowsingStoreFile();

  virtual void Init(const base::FilePath& filename,
                    const base::Closure& corruption_callback) OVERRIDE;

  
  virtual bool Delete() OVERRIDE;

  
  
  virtual bool GetAddPrefixes(SBAddPrefixes* add_prefixes) OVERRIDE;
  virtual bool GetAddFullHashes(
      std::vector<SBAddFullHash>* add_full_hashes) OVERRIDE;

  virtual bool BeginChunk() OVERRIDE;

  virtual bool WriteAddPrefix(int32 chunk_id, SBPrefix prefix) OVERRIDE;
  virtual bool WriteAddHash(int32 chunk_id,
                            base::Time receive_time,
                            const SBFullHash& full_hash) OVERRIDE;
  virtual bool WriteSubPrefix(int32 chunk_id,
                              int32 add_chunk_id, SBPrefix prefix) OVERRIDE;
  virtual bool WriteSubHash(int32 chunk_id, int32 add_chunk_id,
                            const SBFullHash& full_hash) OVERRIDE;
  virtual bool FinishChunk() OVERRIDE;

  virtual bool BeginUpdate() OVERRIDE;
  
  
  virtual bool FinishUpdate(
      const std::vector<SBAddFullHash>& pending_adds,
      SBAddPrefixes* add_prefixes_result,
      std::vector<SBAddFullHash>* add_full_hashes_result) OVERRIDE;
  virtual bool CancelUpdate() OVERRIDE;

  virtual void SetAddChunk(int32 chunk_id) OVERRIDE;
  virtual bool CheckAddChunk(int32 chunk_id) OVERRIDE;
  virtual void GetAddChunks(std::vector<int32>* out) OVERRIDE;
  virtual void SetSubChunk(int32 chunk_id) OVERRIDE;
  virtual bool CheckSubChunk(int32 chunk_id) OVERRIDE;
  virtual void GetSubChunks(std::vector<int32>* out) OVERRIDE;

  virtual void DeleteAddChunk(int32 chunk_id) OVERRIDE;
  virtual void DeleteSubChunk(int32 chunk_id) OVERRIDE;

  
  
  virtual bool CheckValidity() OVERRIDE;

  
  
  static const base::FilePath TemporaryFileForFilename(
      const base::FilePath& filename) {
    return base::FilePath(filename.value() + FILE_PATH_LITERAL("_new"));
  }

  
  static bool DeleteStore(const base::FilePath& basename);

 private:
  
  virtual bool DoUpdate(const std::vector<SBAddFullHash>& pending_adds,
                        SBAddPrefixes* add_prefixes_result,
                        std::vector<SBAddFullHash>* add_full_hashes_result);

  
  
  
  enum FormatEventType {
    
    FORMAT_EVENT_FILE_CORRUPT,
    FORMAT_EVENT_SQLITE_CORRUPT,  

    
    
    FORMAT_EVENT_FOUND_SQLITE,
    FORMAT_EVENT_FOUND_UNKNOWN,

    
    
    
    FORMAT_EVENT_SQLITE_DELETED,  
    FORMAT_EVENT_SQLITE_DELETE_FAILED,  

    
    
    FORMAT_EVENT_DELETED_ORIGINAL,
    FORMAT_EVENT_DELETED_ORIGINAL_FAILED,

    
    
    
    FORMAT_EVENT_VALIDITY_CHECKSUM_FAILURE,
    FORMAT_EVENT_UPDATE_CHECKSUM_FAILURE,

    
    
    FORMAT_EVENT_MAX
  };

  
  
  static void RecordFormatEvent(FormatEventType event_type);

  
  
  
  
  
  static void CheckForOriginalAndDelete(const base::FilePath& filename);

  
  bool Close();

  
  
  bool OnCorruptDatabase();

  
  
  void HandleCorruptDatabase();

  
  bool ClearChunkBuffers() {
    
    
    
    
    SBAddPrefixes().swap(add_prefixes_);
    SBSubPrefixes().swap(sub_prefixes_);
    std::vector<SBAddFullHash>().swap(add_hashes_);
    std::vector<SBSubFullHash>().swap(sub_hashes_);
    return true;
  }

  
  void ClearUpdateBuffers() {
    ClearChunkBuffers();
    chunks_written_ = 0;
    std::set<int32>().swap(add_chunks_cache_);
    std::set<int32>().swap(sub_chunks_cache_);
    base::hash_set<int32>().swap(add_del_cache_);
    base::hash_set<int32>().swap(sub_del_cache_);
  }

  
  
  SBAddPrefixes add_prefixes_;
  SBSubPrefixes sub_prefixes_;
  std::vector<SBAddFullHash> add_hashes_;
  std::vector<SBSubFullHash> sub_hashes_;

  
  int chunks_written_;

  
  base::FilePath filename_;

  
  
  file_util::ScopedFILE file_;
  file_util::ScopedFILE new_file_;
  bool empty_;

  
  
  
  std::set<int32> add_chunks_cache_;
  std::set<int32> sub_chunks_cache_;

  
  
  
  
  base::hash_set<int32> add_del_cache_;
  base::hash_set<int32> sub_del_cache_;

  base::Closure corruption_callback_;

  
  
  
  bool corruption_seen_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingStoreFile);
};

#endif  
