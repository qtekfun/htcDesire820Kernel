// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_DATABASE_H_
#define CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_DATABASE_H_
#pragma once

#include <set>
#include <vector>

#include "base/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/task.h"
#include "chrome/browser/safe_browsing/safe_browsing_store.h"
#include "testing/gtest/include/gtest/gtest_prod.h"

namespace base {
  class Time;
}

namespace safe_browsing {
class PrefixSet;
}

class BloomFilter;
class GURL;
class MessageLoop;
class SafeBrowsingDatabase;

class SafeBrowsingDatabaseFactory {
 public:
  SafeBrowsingDatabaseFactory() { }
  virtual ~SafeBrowsingDatabaseFactory() { }
  virtual SafeBrowsingDatabase* CreateSafeBrowsingDatabase(
      bool enable_download_protection,
      bool enable_client_side_whitelist) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingDatabaseFactory);
};


class SafeBrowsingDatabase {
 public:
  
  
  
  
  
  
  static SafeBrowsingDatabase* Create(bool enable_download_protection,
                                      bool enable_client_side_whitelist);

  
  
  static void RegisterFactory(SafeBrowsingDatabaseFactory* factory) {
    factory_ = factory;
  }

  virtual ~SafeBrowsingDatabase();

  
  virtual void Init(const FilePath& filename) = 0;

  
  virtual bool ResetDatabase() = 0;

  
  
  
  
  
  virtual bool ContainsBrowseUrl(const GURL& url,
                                 std::string* matching_list,
                                 std::vector<SBPrefix>* prefix_hits,
                                 std::vector<SBFullHashResult>* full_hits,
                                 base::Time last_update) = 0;

  
  
  
  virtual bool ContainsDownloadUrl(const std::vector<GURL>& urls,
                                   std::vector<SBPrefix>* prefix_hits) = 0;

  
  
  virtual bool ContainsDownloadHashPrefix(const SBPrefix& prefix) = 0;

  
  
  
  
  virtual bool ContainsCsdWhitelistedUrl(const GURL& url) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool UpdateStarted(std::vector<SBListChunkRanges>* lists) = 0;
  virtual void InsertChunks(const std::string& list_name,
                            const SBChunkList& chunks) = 0;
  virtual void DeleteChunks(
      const std::vector<SBChunkDelete>& chunk_deletes) = 0;
  virtual void UpdateFinished(bool update_succeeded) = 0;

  
  
  
  virtual void CacheHashResults(
      const std::vector<SBPrefix>& prefixes,
      const std::vector<SBFullHashResult>& full_hits) = 0;

  
  static FilePath BloomFilterForFilename(const FilePath& db_filename);

  
  static FilePath BrowseDBFilename(const FilePath& db_base_filename);

  
  static FilePath DownloadDBFilename(const FilePath& db_base_filename);

  
  static FilePath CsdWhitelistDBFilename(
      const FilePath& csd_whitelist_base_filename);

  
  
  enum FailureType {
    FAILURE_DATABASE_CORRUPT,
    FAILURE_DATABASE_CORRUPT_HANDLER,
    FAILURE_BROWSE_DATABASE_UPDATE_BEGIN,
    FAILURE_BROWSE_DATABASE_UPDATE_FINISH,
    FAILURE_DATABASE_FILTER_MISSING,
    FAILURE_DATABASE_FILTER_READ,
    FAILURE_DATABASE_FILTER_WRITE,
    FAILURE_DATABASE_FILTER_DELETE,
    FAILURE_DATABASE_STORE_MISSING,
    FAILURE_DATABASE_STORE_DELETE,
    FAILURE_DOWNLOAD_DATABASE_UPDATE_BEGIN,
    FAILURE_DOWNLOAD_DATABASE_UPDATE_FINISH,
    FAILURE_CSD_WHITELIST_DATABASE_UPDATE_BEGIN,
    FAILURE_CSD_WHITELIST_DATABASE_UPDATE_FINISH,

    
    
    FAILURE_DATABASE_MAX
  };

  static void RecordFailure(FailureType failure_type);

 private:
  
  
  
  static SafeBrowsingDatabaseFactory* factory_;
};

class SafeBrowsingDatabaseNew : public SafeBrowsingDatabase {
 public:
  
  
  
  
  
  SafeBrowsingDatabaseNew(SafeBrowsingStore* browse_store,
                          SafeBrowsingStore* download_store,
                          SafeBrowsingStore* csd_whitelist_store);

  
  
  SafeBrowsingDatabaseNew();

  virtual ~SafeBrowsingDatabaseNew();

  
  virtual void Init(const FilePath& filename);
  virtual bool ResetDatabase();
  virtual bool ContainsBrowseUrl(const GURL& url,
                                 std::string* matching_list,
                                 std::vector<SBPrefix>* prefix_hits,
                                 std::vector<SBFullHashResult>* full_hits,
                                 base::Time last_update);
  virtual bool ContainsDownloadUrl(const std::vector<GURL>& urls,
                                   std::vector<SBPrefix>* prefix_hits);
  virtual bool ContainsDownloadHashPrefix(const SBPrefix& prefix);
  virtual bool ContainsCsdWhitelistedUrl(const GURL& url);
  virtual bool UpdateStarted(std::vector<SBListChunkRanges>* lists);
  virtual void InsertChunks(const std::string& list_name,
                            const SBChunkList& chunks);
  virtual void DeleteChunks(const std::vector<SBChunkDelete>& chunk_deletes);
  virtual void UpdateFinished(bool update_succeeded);
  virtual void CacheHashResults(const std::vector<SBPrefix>& prefixes,
                                const std::vector<SBFullHashResult>& full_hits);

 private:
  friend class SafeBrowsingDatabaseTest;
  FRIEND_TEST(SafeBrowsingDatabaseTest, HashCaching);

  
  
  SafeBrowsingStore* GetStore(int list_id);

    
  bool Delete();

  
  void LoadBloomFilter();

  
  void WriteBloomFilter();

  
  
  
  void LoadCsdWhitelist(const std::vector<SBAddFullHash>& full_hashes);

  
  
  void CsdWhitelistAllUrls();

  
  
  
  
  
  
  
  void HandleCorruptDatabase();
  void OnHandleCorruptDatabase();

  
  void InsertAdd(int chunk, SBPrefix host, const SBEntry* entry, int list_id);
  void InsertAddChunks(int list_id, const SBChunkList& chunks);
  void InsertSub(int chunk, SBPrefix host, const SBEntry* entry, int list_id);
  void InsertSubChunks(int list_id, const SBChunkList& chunks);

  void UpdateDownloadStore();
  void UpdateBrowseStore();
  void UpdateCsdWhitelistStore();

  
  
  
  
  
  bool MatchDownloadAddPrefixes(int list_bit,
                                const std::vector<SBPrefix>& prefixes,
                                std::vector<SBPrefix>* prefix_hits);

  
  
  MessageLoop* creation_loop_;

  
  
  
  
  base::Lock lookup_lock_;

  
  
  FilePath browse_filename_;
  scoped_ptr<SafeBrowsingStore> browse_store_;

  
  FilePath download_filename_;
  scoped_ptr<SafeBrowsingStore> download_store_;

  
  
  FilePath csd_whitelist_filename_;
  scoped_ptr<SafeBrowsingStore> csd_whitelist_store_;

  
  
  std::vector<SBFullHash> csd_whitelist_;

  
  
  
  
  bool csd_whitelist_all_urls_;

  
  
  FilePath bloom_filter_filename_;
  scoped_refptr<BloomFilter> browse_bloom_filter_;

  
  
  
  
  
  std::vector<SBAddFullHash> full_browse_hashes_;
  std::vector<SBAddFullHash> pending_browse_hashes_;

  
  
  
  std::set<SBPrefix> prefix_miss_cache_;

  
  ScopedRunnableMethodFactory<SafeBrowsingDatabaseNew> reset_factory_;

  
  
  
  bool corruption_detected_;

  
  
  bool change_detected_;

  
  scoped_ptr<safe_browsing::PrefixSet> prefix_set_;
};

#endif  
