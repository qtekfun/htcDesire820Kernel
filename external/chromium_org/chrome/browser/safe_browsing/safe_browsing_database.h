// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_DATABASE_H_
#define CHROME_BROWSER_SAFE_BROWSING_SAFE_BROWSING_DATABASE_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/safe_browsing/safe_browsing_store.h"

namespace base {
class MessageLoop;
class Time;
}

namespace safe_browsing {
class PrefixSet;
}

class GURL;
class SafeBrowsingDatabase;

class SafeBrowsingDatabaseFactory {
 public:
  SafeBrowsingDatabaseFactory() { }
  virtual ~SafeBrowsingDatabaseFactory() { }
  virtual SafeBrowsingDatabase* CreateSafeBrowsingDatabase(
      bool enable_download_protection,
      bool enable_client_side_whitelist,
      bool enable_download_whitelist,
      bool enable_extension_blacklist,
      bool enable_side_effect_free_whitelist,
      bool enable_ip_blacklist) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(SafeBrowsingDatabaseFactory);
};


class SafeBrowsingDatabase {
 public:
  
  
  
  
  
  
  
  
  
  
  static SafeBrowsingDatabase* Create(bool enable_download_protection,
                                      bool enable_client_side_whitelist,
                                      bool enable_download_whitelist,
                                      bool enable_extension_blacklist,
                                      bool side_effect_free_whitelist,
                                      bool enable_ip_blacklist);

  
  
  static void RegisterFactory(SafeBrowsingDatabaseFactory* factory) {
    factory_ = factory;
  }

  virtual ~SafeBrowsingDatabase();

  
  virtual void Init(const base::FilePath& filename) = 0;

  
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

  
  
  
  
  
  
  
  virtual bool ContainsDownloadWhitelistedUrl(const GURL& url) = 0;
  virtual bool ContainsDownloadWhitelistedString(const std::string& str) = 0;

  
  
  
  
  virtual bool ContainsExtensionPrefixes(
      const std::vector<SBPrefix>& prefixes,
      std::vector<SBPrefix>* prefix_hits) = 0;

  
  
  virtual bool ContainsSideEffectFreeWhitelistUrl(const GURL& url) = 0;

  
  virtual bool ContainsMalwareIP(const std::string& ip_address) = 0;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool UpdateStarted(std::vector<SBListChunkRanges>* lists) = 0;
  virtual void InsertChunks(const std::string& list_name,
                            const SBChunkList& chunks) = 0;
  virtual void DeleteChunks(
      const std::vector<SBChunkDelete>& chunk_deletes) = 0;
  virtual void UpdateFinished(bool update_succeeded) = 0;

  
  
  
  virtual void CacheHashResults(
      const std::vector<SBPrefix>& prefixes,
      const std::vector<SBFullHashResult>& full_hits) = 0;

  
  
  virtual bool IsMalwareIPMatchKillSwitchOn() = 0;

  
  
  static base::FilePath BloomFilterForFilename(
      const base::FilePath& db_filename);

  
  static base::FilePath PrefixSetForFilename(const base::FilePath& db_filename);

  
  static base::FilePath BrowseDBFilename(
      const base::FilePath& db_base_filename);

  
  static base::FilePath DownloadDBFilename(
      const base::FilePath& db_base_filename);

  
  static base::FilePath CsdWhitelistDBFilename(
      const base::FilePath& csd_whitelist_base_filename);

  
  static base::FilePath DownloadWhitelistDBFilename(
      const base::FilePath& download_whitelist_base_filename);

  
  static base::FilePath ExtensionBlacklistDBFilename(
      const base::FilePath& extension_blacklist_base_filename);

  
  static base::FilePath SideEffectFreeWhitelistDBFilename(
      const base::FilePath& side_effect_free_whitelist_base_filename);

  
  static base::FilePath IpBlacklistDBFilename(
      const base::FilePath& ip_blacklist_base_filename);

  
  
  enum FailureType {
    FAILURE_DATABASE_CORRUPT,
    FAILURE_DATABASE_CORRUPT_HANDLER,
    FAILURE_BROWSE_DATABASE_UPDATE_BEGIN,
    FAILURE_BROWSE_DATABASE_UPDATE_FINISH,
    FAILURE_DATABASE_FILTER_MISSING_OBSOLETE,
    FAILURE_DATABASE_FILTER_READ_OBSOLETE,
    FAILURE_DATABASE_FILTER_WRITE_OBSOLETE,
    FAILURE_DATABASE_FILTER_DELETE,
    FAILURE_DATABASE_STORE_MISSING,
    FAILURE_DATABASE_STORE_DELETE,
    FAILURE_DOWNLOAD_DATABASE_UPDATE_BEGIN,
    FAILURE_DOWNLOAD_DATABASE_UPDATE_FINISH,
    FAILURE_WHITELIST_DATABASE_UPDATE_BEGIN,
    FAILURE_WHITELIST_DATABASE_UPDATE_FINISH,
    FAILURE_BROWSE_PREFIX_SET_MISSING,
    FAILURE_BROWSE_PREFIX_SET_READ,
    FAILURE_BROWSE_PREFIX_SET_WRITE,
    FAILURE_BROWSE_PREFIX_SET_DELETE,
    FAILURE_EXTENSION_BLACKLIST_UPDATE_BEGIN,
    FAILURE_EXTENSION_BLACKLIST_UPDATE_FINISH,
    FAILURE_EXTENSION_BLACKLIST_DELETE,
    FAILURE_SIDE_EFFECT_FREE_WHITELIST_UPDATE_BEGIN,
    FAILURE_SIDE_EFFECT_FREE_WHITELIST_UPDATE_FINISH,
    FAILURE_SIDE_EFFECT_FREE_WHITELIST_DELETE,
    FAILURE_SIDE_EFFECT_FREE_WHITELIST_PREFIX_SET_READ,
    FAILURE_SIDE_EFFECT_FREE_WHITELIST_PREFIX_SET_WRITE,
    FAILURE_SIDE_EFFECT_FREE_WHITELIST_PREFIX_SET_DELETE,
    FAILURE_IP_BLACKLIST_UPDATE_BEGIN,
    FAILURE_IP_BLACKLIST_UPDATE_FINISH,
    FAILURE_IP_BLACKLIST_UPDATE_INVALID,
    FAILURE_IP_BLACKLIST_DELETE,

    
    
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
                          SafeBrowsingStore* csd_whitelist_store,
                          SafeBrowsingStore* download_whitelist_store,
                          SafeBrowsingStore* extension_blacklist_store,
                          SafeBrowsingStore* side_effect_free_whitelist_store,
                          SafeBrowsingStore* ip_blacklist_store);

  
  
  SafeBrowsingDatabaseNew();

  virtual ~SafeBrowsingDatabaseNew();

  
  virtual void Init(const base::FilePath& filename) OVERRIDE;
  virtual bool ResetDatabase() OVERRIDE;
  virtual bool ContainsBrowseUrl(const GURL& url,
                                 std::string* matching_list,
                                 std::vector<SBPrefix>* prefix_hits,
                                 std::vector<SBFullHashResult>* full_hits,
                                 base::Time last_update) OVERRIDE;
  virtual bool ContainsDownloadUrl(const std::vector<GURL>& urls,
                                   std::vector<SBPrefix>* prefix_hits) OVERRIDE;
  virtual bool ContainsDownloadHashPrefix(const SBPrefix& prefix) OVERRIDE;
  virtual bool ContainsCsdWhitelistedUrl(const GURL& url) OVERRIDE;
  virtual bool ContainsDownloadWhitelistedUrl(const GURL& url) OVERRIDE;
  virtual bool ContainsDownloadWhitelistedString(
      const std::string& str) OVERRIDE;
  virtual bool ContainsExtensionPrefixes(
      const std::vector<SBPrefix>& prefixes,
      std::vector<SBPrefix>* prefix_hits) OVERRIDE;
  virtual bool ContainsSideEffectFreeWhitelistUrl(const GURL& url)  OVERRIDE;
  virtual bool ContainsMalwareIP(const std::string& ip_address) OVERRIDE;
  virtual bool UpdateStarted(std::vector<SBListChunkRanges>* lists) OVERRIDE;
  virtual void InsertChunks(const std::string& list_name,
                            const SBChunkList& chunks) OVERRIDE;
  virtual void DeleteChunks(
      const std::vector<SBChunkDelete>& chunk_deletes) OVERRIDE;
  virtual void UpdateFinished(bool update_succeeded) OVERRIDE;
  virtual void CacheHashResults(
      const std::vector<SBPrefix>& prefixes,
      const std::vector<SBFullHashResult>& full_hits) OVERRIDE;

  
  virtual bool IsMalwareIPMatchKillSwitchOn() OVERRIDE;

 private:
  friend class SafeBrowsingDatabaseTest;
  FRIEND_TEST_ALL_PREFIXES(SafeBrowsingDatabaseTest, HashCaching);

  
  
  
  typedef std::pair<std::vector<SBFullHash>, bool> SBWhitelist;

  
  
  
  typedef std::map<std::string, base::hash_set<std::string> > IPBlacklist;

  
  
  bool ContainsWhitelistedHashes(const SBWhitelist& whitelist,
                                 const std::vector<SBFullHash>& hashes);

  
  
  SafeBrowsingStore* GetStore(int list_id);

  
  bool Delete();

  
  void LoadPrefixSet();

  
  void WritePrefixSet();

  
  
  
  void LoadWhitelist(const std::vector<SBAddFullHash>& full_hashes,
                     SBWhitelist* whitelist);

  
  
  void WhitelistEverything(SBWhitelist* whitelist);

  
  void LoadIpBlacklist(const std::vector<SBAddFullHash>& full_hashes);

  
  
  
  
  
  
  
  void HandleCorruptDatabase();
  void OnHandleCorruptDatabase();

  
  void InsertAdd(int chunk, SBPrefix host, const SBEntry* entry, int list_id);
  void InsertAddChunks(safe_browsing_util::ListType list_id,
                       const SBChunkList& chunks);
  void InsertSub(int chunk, SBPrefix host, const SBEntry* entry, int list_id);
  void InsertSubChunks(safe_browsing_util::ListType list_id,
                       const SBChunkList& chunks);

  
  int64 UpdateHashPrefixStore(const base::FilePath& store_filename,
                               SafeBrowsingStore* store,
                               FailureType failure_type);
  void UpdateBrowseStore();
  void UpdateSideEffectFreeWhitelistStore();
  void UpdateWhitelistStore(const base::FilePath& store_filename,
                            SafeBrowsingStore* store,
                            SBWhitelist* whitelist);
  void UpdateIpBlacklistStore();

  
  
  base::MessageLoop* creation_loop_;

  
  
  
  base::Lock lookup_lock_;

  
  
  base::FilePath browse_filename_;
  scoped_ptr<SafeBrowsingStore> browse_store_;

  
  base::FilePath download_filename_;
  scoped_ptr<SafeBrowsingStore> download_store_;

  
  
  base::FilePath csd_whitelist_filename_;
  scoped_ptr<SafeBrowsingStore> csd_whitelist_store_;

  
  
  base::FilePath download_whitelist_filename_;
  scoped_ptr<SafeBrowsingStore> download_whitelist_store_;

  
  base::FilePath extension_blacklist_filename_;
  scoped_ptr<SafeBrowsingStore> extension_blacklist_store_;

  
  base::FilePath side_effect_free_whitelist_filename_;
  scoped_ptr<SafeBrowsingStore> side_effect_free_whitelist_store_;

  
  base::FilePath ip_blacklist_filename_;
  scoped_ptr<SafeBrowsingStore> ip_blacklist_store_;

  SBWhitelist csd_whitelist_;
  SBWhitelist download_whitelist_;
  SBWhitelist extension_blacklist_;

  
  IPBlacklist ip_blacklist_;

  
  
  
  
  
  std::vector<SBAddFullHash> full_browse_hashes_;
  std::vector<SBAddFullHash> pending_browse_hashes_;

  
  
  
  std::set<SBPrefix> prefix_miss_cache_;

  
  base::WeakPtrFactory<SafeBrowsingDatabaseNew> reset_factory_;

  
  
  
  bool corruption_detected_;

  
  
  bool change_detected_;

  
  base::FilePath browse_prefix_set_filename_;
  scoped_ptr<safe_browsing::PrefixSet> browse_prefix_set_;

  
  base::FilePath side_effect_free_whitelist_prefix_set_filename_;
  scoped_ptr<safe_browsing::PrefixSet> side_effect_free_whitelist_prefix_set_;
};

#endif  
