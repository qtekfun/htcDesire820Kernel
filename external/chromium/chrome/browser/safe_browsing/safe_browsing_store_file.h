// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_STORE_FILE_H_
#define CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_STORE_FILE_H_
#pragma once

#include <set>
#include <vector>

#include "chrome/browser/safe_browsing/safe_browsing_store.h"

#include "base/callback.h"
#include "base/file_util.h"



class SafeBrowsingStoreFile : public SafeBrowsingStore {
 public:
  SafeBrowsingStoreFile();
  virtual ~SafeBrowsingStoreFile();

  virtual void Init(const FilePath& filename,
                    Callback0::Type* corruption_callback);

  
  virtual bool Delete();

  
  
  virtual bool GetAddPrefixes(std::vector<SBAddPrefix>* add_prefixes);
  virtual bool GetAddFullHashes(std::vector<SBAddFullHash>* add_full_hashes);

  virtual bool BeginChunk();

  virtual bool WriteAddPrefix(int32 chunk_id, SBPrefix prefix);
  virtual bool WriteAddHash(int32 chunk_id,
                            base::Time receive_time,
                            const SBFullHash& full_hash);
  virtual bool WriteSubPrefix(int32 chunk_id,
                              int32 add_chunk_id, SBPrefix prefix);
  virtual bool WriteSubHash(int32 chunk_id, int32 add_chunk_id,
                            const SBFullHash& full_hash);
  virtual bool FinishChunk();

  virtual bool BeginUpdate();
  
  
  virtual bool FinishUpdate(const std::vector<SBAddFullHash>& pending_adds,
                            const std::set<SBPrefix>& prefix_misses,
                            std::vector<SBAddPrefix>* add_prefixes_result,
                            std::vector<SBAddFullHash>* add_full_hashes_result);
  virtual bool CancelUpdate();

  virtual void SetAddChunk(int32 chunk_id);
  virtual bool CheckAddChunk(int32 chunk_id);
  virtual void GetAddChunks(std::vector<int32>* out);
  virtual void SetSubChunk(int32 chunk_id);
  virtual bool CheckSubChunk(int32 chunk_id);
  virtual void GetSubChunks(std::vector<int32>* out);

  virtual void DeleteAddChunk(int32 chunk_id);
  virtual void DeleteSubChunk(int32 chunk_id);

  
  
  static const FilePath TemporaryFileForFilename(const FilePath& filename) {
    return FilePath(filename.value() + FILE_PATH_LITERAL("_new"));
  }

 private:
  
  virtual bool DoUpdate(const std::vector<SBAddFullHash>& pending_adds,
                        const std::set<SBPrefix>& prefix_misses,
                        std::vector<SBAddPrefix>* add_prefixes_result,
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

    
    
    FORMAT_EVENT_MAX
  };

  
  
  static void RecordFormatEvent(FormatEventType event_type);

  
  
  
  
  
  static void CheckForOriginalAndDelete(const FilePath& filename);

  
  bool Close();

  
  
  bool OnCorruptDatabase();

  
  
  void HandleCorruptDatabase();

  
  bool ClearChunkBuffers() {
    
    
    
    
    std::vector<SBAddPrefix>().swap(add_prefixes_);
    std::vector<SBSubPrefix>().swap(sub_prefixes_);
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

  
  
  std::vector<SBAddPrefix> add_prefixes_;
  std::vector<SBSubPrefix> sub_prefixes_;
  std::vector<SBAddFullHash> add_hashes_;
  std::vector<SBSubFullHash> sub_hashes_;

  
  int chunks_written_;

  
  FilePath filename_;

  
  
  file_util::ScopedFILE file_;
  file_util::ScopedFILE new_file_;
  bool empty_;

  
  
  
  std::set<int32> add_chunks_cache_;
  std::set<int32> sub_chunks_cache_;

  
  
  
  
  base::hash_set<int32> add_del_cache_;
  base::hash_set<int32> sub_del_cache_;

  scoped_ptr<Callback0::Type> corruption_callback_;

  
  
  
  bool corruption_seen_;

  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingStoreFile);
};

#endif  
