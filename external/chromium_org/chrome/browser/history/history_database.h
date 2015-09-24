// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_HISTORY_DATABASE_H_
#define CHROME_BROWSER_HISTORY_HISTORY_DATABASE_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "build/build_config.h"
#include "chrome/browser/history/download_database.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/url_database.h"
#include "chrome/browser/history/visit_database.h"
#include "chrome/browser/history/visitsegment_database.h"
#include "sql/connection.h"
#include "sql/init_status.h"
#include "sql/meta_table.h"

#if defined(OS_ANDROID)
#include "chrome/browser/history/android/android_cache_database.h"
#include "chrome/browser/history/android/android_urls_database.h"
#endif

namespace base {
class FilePath;
}

class HistoryQuickProviderTest;

namespace history {

class HistoryDatabase : public DownloadDatabase,
#if defined(OS_ANDROID)
                        public AndroidURLsDatabase,
                        public AndroidCacheDatabase,
#endif
                        public URLDatabase,
                        public VisitDatabase,
                        public VisitSegmentDatabase {
 public:
  
  
  class TransactionScoper {
   public:
    explicit TransactionScoper(HistoryDatabase* db) : db_(db) {
      db_->BeginTransaction();
    }
    ~TransactionScoper() {
      db_->CommitTransaction();
    }
   private:
    HistoryDatabase* db_;
  };

  
  
  
  HistoryDatabase();

  virtual ~HistoryDatabase();

  
  
  void set_error_callback(
      const sql::Connection::ErrorCallback& error_callback) {
    error_callback_ = error_callback;
  }

  
  
  
  sql::InitStatus Init(const base::FilePath& history_name);

  
  
  void ComputeDatabaseMetrics(const base::FilePath& filename);

  
  
  
  
  
  void BeginExclusiveMode();

  
  static int GetCurrentVersion();

  
  
  
  
  
  
  
  void BeginTransaction();
  void CommitTransaction();
  int transaction_nesting() const {  
    return db_.transaction_nesting();
  }
  void RollbackTransaction();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool RecreateAllTablesButURL();

  
  
  void Vacuum();

  
  
  void TrimMemory(bool aggressively);

  
  bool Raze();

  
  
  
  
  
  
  
  bool needs_version_17_migration() const {
    return needs_version_17_migration_;
  }

  

  
  bool SetSegmentID(VisitID visit_id, SegmentID segment_id);

  
  
  SegmentID GetSegmentID(VisitID visit_id);

  
  
  
  virtual base::Time GetEarlyExpirationThreshold();
  virtual void UpdateEarlyExpirationThreshold(base::Time threshold);

 private:
#if defined(OS_ANDROID)
  
  friend class AndroidProviderBackend;
  FRIEND_TEST_ALL_PREFIXES(AndroidURLsMigrationTest, MigrateToVersion22);
#endif
  friend class ::HistoryQuickProviderTest;
  friend class InMemoryURLIndexTest;

  
  virtual sql::Connection& GetDB() OVERRIDE;

  

  
  
  
  
  
  
  sql::InitStatus EnsureCurrentVersion();

#if !defined(OS_WIN)
  
  
  void MigrateTimeEpoch();
#endif

  

  sql::Connection::ErrorCallback error_callback_;
  sql::Connection db_;
  sql::MetaTable meta_table_;

  base::Time cached_early_expiration_threshold_;

  
  bool needs_version_17_migration_;

  DISALLOW_COPY_AND_ASSIGN(HistoryDatabase);
};

}  

#endif  
