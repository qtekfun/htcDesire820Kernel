// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_ANDROID_ANDROID_PROVIDER_BACKEND_H_
#define CHROME_BROWSER_HISTORY_ANDROID_ANDROID_PROVIDER_BACKEND_H_

#include <set>

#include "base/containers/hash_tables.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/history/android/android_cache_database.h"
#include "chrome/browser/history/android/android_history_types.h"
#include "chrome/browser/history/android/sql_handler.h"
#include "chrome/browser/history/history_backend.h"
#include "chrome/browser/history/history_notifications.h"
#include "sql/statement.h"
#include "sql/transaction.h"

class BookmarkService;

namespace history {

class AndroidProviderBackend;
class AndroidURLsSQLHandler;
class HistoryDatabase;
class ThumbnailDatabase;

class AndroidProviderBackend {
 public:
  AndroidProviderBackend(const base::FilePath& cache_db_name,
                         HistoryDatabase* history_db,
                         ThumbnailDatabase* thumbnail_db,
                         BookmarkService* bookmark_service,
                         HistoryBackend::Delegate* delegate);

  ~AndroidProviderBackend();

  
  
  
  
  
  
  
  
  
  AndroidStatement* QueryHistoryAndBookmarks(
      const std::vector<HistoryAndBookmarkRow::ColumnID>& projections,
      const std::string& selection,
      const std::vector<base::string16>& selection_args,
      const std::string& sort_order);

  
  
  
  
  
  
  bool UpdateHistoryAndBookmarks(
      const HistoryAndBookmarkRow& row,
      const std::string& selection,
      const std::vector<base::string16>& selection_args,
      int* update_count);

  
  AndroidURLID InsertHistoryAndBookmark(const HistoryAndBookmarkRow& values);

  
  
  
  
  
  
  bool DeleteHistoryAndBookmarks(
      const std::string& selection,
      const std::vector<base::string16>& selection_args,
      int* deleted_count);

  
  
  
  
  bool DeleteHistory(const std::string& selection,
                     const std::vector<base::string16>& selection_args,
                     int* deleted_count);

  
  
  
  
  
  
  
  
  AndroidStatement* QuerySearchTerms(
      const std::vector<SearchRow::ColumnID>& projections,
      const std::string& selection,
      const std::vector<base::string16>& selection_args,
      const std::string& sort_order);

  
  
  
  
  
  
  bool UpdateSearchTerms(const SearchRow& row,
                         const std::string& selection,
                         const std::vector<base::string16>& selection_args,
                         int* update_count);

  
  SearchTermID InsertSearchTerm(const SearchRow& values);

  
  
  
  
  
  
  bool DeleteSearchTerms(const std::string& selection,
                         const std::vector<base::string16>& selection_args,
                         int * deleted_count);

 private:
  friend class AndroidProviderBackendTest;

  FRIEND_TEST_ALL_PREFIXES(AndroidProviderBackendTest, UpdateTables);
  FRIEND_TEST_ALL_PREFIXES(AndroidProviderBackendTest, UpdateSearchTermTable);

  struct HistoryNotification {
    HistoryNotification(int type, HistoryDetails* detail);
    ~HistoryNotification();

    int type;
    
    HistoryDetails* detail;
  };
  typedef std::vector<HistoryNotification> HistoryNotifications;

  
  
  
  
  
  
  
  
  
  
  
  
  
  class ScopedTransaction {
   public:
    ScopedTransaction(HistoryDatabase* history_db,
                      ThumbnailDatabase* thumbnail_db);
    ~ScopedTransaction();

    
    void Commit();

   private:
    HistoryDatabase* history_db_;
    ThumbnailDatabase* thumbnail_db_;
    
    bool committed_;
    
    const int history_transaction_nesting_;
    
    const int thumbnail_transaction_nesting_;

    DISALLOW_COPY_AND_ASSIGN(ScopedTransaction);
  };

  
  
  
  
  
  
  
  
  
  bool UpdateHistoryAndBookmarks(const HistoryAndBookmarkRow& row,
                       const std::string& selection,
                       const std::vector<base::string16>& selection_args,
                       int* update_count,
                       HistoryNotifications* notifications);

  
  
  
  
  
  AndroidURLID InsertHistoryAndBookmark(const HistoryAndBookmarkRow& values,
                                        HistoryNotifications* notifications,
                                        bool ensure_initialized_and_updated);

  
  
  
  
  
  
  
  
  bool DeleteHistoryAndBookmarks(
      const std::string& selection,
      const std::vector<base::string16>& selection_args,
      int* deleted_count,
      HistoryNotifications* notifications);

  
  
  
  
  
  
  bool DeleteHistory(const std::string& selection,
                     const std::vector<base::string16>& selection_args,
                     int* deleted_count,
                     HistoryNotifications* notifications);

  
  bool EnsureInitializedAndUpdated();

  
  bool Init();

  
  bool UpdateTables();

  
  bool UpdateVisitedURLs();

  
  bool UpdateRemovedURLs();

  
  bool UpdateBookmarks();

  
  bool UpdateFavicon();

  
  bool UpdateSearchTermTable();

  
  
  
  
  int AppendBookmarkResultColumn(
      const std::vector<HistoryAndBookmarkRow::ColumnID>& projections,
      std::string* result_column);

  
  
  void AppendSearchResultColumn(
      const std::vector<SearchRow::ColumnID>& projections,
      std::string* result_column);

  
  
  bool GetSelectedURLs(const std::string& selection,
                       const std::vector<base::string16>& selection_args,
                       TableIDRows* rows);

  
  
  typedef std::vector<base::string16> SearchTerms;
  bool GetSelectedSearchTerms(const std::string& selection,
                              const std::vector<base::string16>& selection_args,
                              SearchTerms* rows);

  
  
  bool SimulateUpdateURL(const HistoryAndBookmarkRow& row,
                         const TableIDRows& ids,
                         HistoryNotifications* notifications);

  
  
  AndroidStatement* QueryHistoryAndBookmarksInternal(
      const std::vector<HistoryAndBookmarkRow::ColumnID>& projections,
      const std::string& selection,
      const std::vector<base::string16>& selection_args,
      const std::string& sort_order);

  
  
  
  
  bool DeleteHistoryInternal(const TableIDRows& urls,
                             bool delete_bookmarks,
                             HistoryNotifications* notifications);

  void BroadcastNotifications(const HistoryNotifications& notifications);

  
  
  
  bool AddSearchTerm(const SearchRow& values);

  
  scoped_ptr<SQLHandler> urls_handler_;
  scoped_ptr<SQLHandler> visit_handler_;
  scoped_ptr<SQLHandler> android_urls_handler_;
  scoped_ptr<SQLHandler> favicon_handler_;
  scoped_ptr<SQLHandler> bookmark_model_handler_;

  
  std::vector<SQLHandler*> sql_handlers_;

  
  const base::FilePath android_cache_db_filename_;

  
  sql::Connection* db_;

  HistoryDatabase* history_db_;

  ThumbnailDatabase* thumbnail_db_;

  BookmarkService* bookmark_service_;

  
  bool initialized_;

  HistoryBackend::Delegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(AndroidProviderBackend);
};

}  

#endif  
