// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_ANDROID_ANDROID_CACHE_DATABASE_H_
#define CHROME_BROWSER_HISTORY_ANDROID_ANDROID_CACHE_DATABASE_H_

#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/time/time.h"
#include "chrome/browser/history/android/android_history_types.h"
#include "sql/connection.h"
#include "sql/init_status.h"

namespace history {

class AndroidCacheDatabase {
 public:
  AndroidCacheDatabase();
  virtual ~AndroidCacheDatabase();

  
  
  
  sql::InitStatus InitAndroidCacheDatabase(const base::FilePath& db_name);

  
  
  
  bool AddBookmarkCacheRow(const base::Time& created_time,
                           const base::Time& last_visit_time,
                           URLID url_id);

  
  bool ClearAllBookmarkCache();

  
  bool MarkURLsAsBookmarked(const std::vector<URLID>& url_id);

  
  
  bool SetFaviconID(URLID url_id, chrome::FaviconID favicon_id);

  
  
  
  
  
  SearchTermID AddSearchTerm(const base::string16& term,
                             const base::Time& last_visit_time);

  
  bool UpdateSearchTerm(SearchTermID id, const SearchTermRow& row);

  
  
  
  SearchTermID GetSearchTerm(const base::string16& term, SearchTermRow* row);

  
  bool DeleteUnusedSearchTerms();

 protected:
  
  
  virtual sql::Connection& GetDB() = 0;

 private:
  FRIEND_TEST_ALL_PREFIXES(AndroidCacheDatabaseTest, InitAndroidCacheDatabase);

  
  
  bool CreateDatabase(const base::FilePath& db_name);

  
  
  
  
  
  
  
  
  
  
  
  
  
  bool CreateBookmarkCacheTable();

  
  
  
  
  
  
  
  
  
  
  
  
  bool CreateSearchTermsTable();

  
  bool Attach();

  
  bool DoAttach();

  base::FilePath db_name_;

  DISALLOW_COPY_AND_ASSIGN(AndroidCacheDatabase);
};

}  

#endif  
