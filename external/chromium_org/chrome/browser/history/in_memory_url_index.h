// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_IN_MEMORY_URL_INDEX_H_
#define CHROME_BROWSER_HISTORY_IN_MEMORY_URL_INDEX_H_

#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/autocomplete/history_provider_util.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/history_db_task.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/in_memory_url_index_types.h"
#include "chrome/browser/history/scored_history_match.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "sql/connection.h"

class HistoryQuickProviderTest;
class Profile;

namespace base {
class Time;
}

namespace in_memory_url_index {
class InMemoryURLIndexCacheItem;
}

namespace history {

namespace imui = in_memory_url_index;

class HistoryDatabase;
class URLIndexPrivateData;
struct URLsDeletedDetails;
struct URLsModifiedDetails;
struct URLVisitedDetails;

class InMemoryURLIndex : public content::NotificationObserver,
                         public base::SupportsWeakPtr<InMemoryURLIndex> {
 public:
  
  
  
  class RestoreCacheObserver {
   public:
    virtual ~RestoreCacheObserver();

    
    
    
    virtual void OnCacheRestoreFinished(bool succeeded) = 0;
  };

  
  
  class SaveCacheObserver {
   public:
    virtual ~SaveCacheObserver();

    
    
    virtual void OnCacheSaveFinished(bool succeeded) = 0;
  };

  
  
  
  
  
  InMemoryURLIndex(Profile* profile,
                   const base::FilePath& history_dir,
                   const std::string& languages);
  virtual ~InMemoryURLIndex();

  
  
  
  void Init();

  
  
  void ShutDown();

  
  
  
  
  
  
  ScoredHistoryMatches HistoryItemsForTerms(const base::string16& term_string,
                                            size_t cursor_position);

  
  void DeleteURL(const GURL& url);

  
  
  void set_restore_cache_observer(
      RestoreCacheObserver* restore_cache_observer) {
    restore_cache_observer_ = restore_cache_observer;
  }
  void set_save_cache_observer(SaveCacheObserver* save_cache_observer) {
    save_cache_observer_ = save_cache_observer;
  }

  
  bool restored() const {
    return restored_;
  }

 private:
  friend class ::HistoryQuickProviderTest;
  friend class InMemoryURLIndexTest;
  friend class InMemoryURLIndexCacheTest;
  FRIEND_TEST_ALL_PREFIXES(LimitedInMemoryURLIndexTest, Initialization);

  
  InMemoryURLIndex();

  
  class RebuildPrivateDataFromHistoryDBTask : public HistoryDBTask {
   public:
    explicit RebuildPrivateDataFromHistoryDBTask(
        InMemoryURLIndex* index,
        const std::string& languages,
        const std::set<std::string>& scheme_whitelist);

    virtual bool RunOnDBThread(HistoryBackend* backend,
                               history::HistoryDatabase* db) OVERRIDE;
    virtual void DoneRunOnMainThread() OVERRIDE;

   private:
    virtual ~RebuildPrivateDataFromHistoryDBTask();

    InMemoryURLIndex* index_;  
    std::string languages_;  
    std::set<std::string> scheme_whitelist_;  
    bool succeeded_;  
    scoped_refptr<URLIndexPrivateData> data_;  

    DISALLOW_COPY_AND_ASSIGN(RebuildPrivateDataFromHistoryDBTask);
  };

  
  
  void ClearPrivateData();

  
  
  
  
  bool GetCacheFilePath(base::FilePath* file_path);

  
  
  void PostRestoreFromCacheFileTask();

  
  
  void ScheduleRebuildFromHistory();

  
  
  
  
  void DoneRebuidingPrivateDataFromHistoryDB(
      bool succeeded,
      scoped_refptr<URLIndexPrivateData> private_data);

  
  
  void RebuildFromHistory(HistoryDatabase* history_db);

  
  
  
  
  
  
  void OnCacheLoadDone(scoped_refptr<URLIndexPrivateData> private_data_ptr);

  
  
  
  void OnCacheRestored(URLIndexPrivateData* private_data);

  
  
  void PostSaveToCacheFileTask();

  
  
  void DoSaveToCacheFile(const base::FilePath& path);

  
  
  void OnCacheSaveDone(bool succeeded);

  
  virtual void Observe(int notification_type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void OnURLVisited(const URLVisitedDetails* details);
  void OnURLsModified(const URLsModifiedDetails* details);
  void OnURLsDeleted(const URLsDeletedDetails* details);

  
  
  void set_history_dir(const base::FilePath& dir_path) {
    history_dir_ = dir_path;
  }

  
  URLIndexPrivateData* private_data() { return private_data_.get(); }

  
  const std::set<std::string>& scheme_whitelist() { return scheme_whitelist_; }

  
  Profile* profile_;

  
  
  
  base::FilePath history_dir_;

  
  std::string languages_;

  
  std::set<std::string> scheme_whitelist_;

  
  scoped_refptr<URLIndexPrivateData> private_data_;

  
  RestoreCacheObserver* restore_cache_observer_;
  SaveCacheObserver* save_cache_observer_;

  CancelableRequestConsumer cache_reader_consumer_;
  content::NotificationRegistrar registrar_;

  
  bool shutdown_;

  
  bool restored_;

  
  
  
  
  
  
  bool needs_to_be_cached_;

  DISALLOW_COPY_AND_ASSIGN(InMemoryURLIndex);
};

}  

#endif  
