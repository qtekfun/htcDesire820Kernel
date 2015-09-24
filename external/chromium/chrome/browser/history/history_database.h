// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_HISTORY_DATABASE_H_
#define CHROME_BROWSER_HISTORY_HISTORY_DATABASE_H_
#pragma once

#include "app/sql/connection.h"
#include "app/sql/init_status.h"
#include "app/sql/meta_table.h"
#include "build/build_config.h"
#include "chrome/browser/history/download_database.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/starred_url_database.h"
#include "chrome/browser/history/url_database.h"
#include "chrome/browser/history/visit_database.h"
#include "chrome/browser/history/visitsegment_database.h"

class FilePath;

namespace history {

class TextDatabaseManager;

class HistoryDatabase : public DownloadDatabase,
  
  
  
                        public StarredURLDatabase,
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

  
  
  
  sql::InitStatus Init(const FilePath& history_name,
                       const FilePath& tmp_bookmarks_path);

  
  
  
  
  
  void BeginExclusiveMode();

  
  static int GetCurrentVersion();

  
  
  
  
  
  
  
  void BeginTransaction();
  void CommitTransaction();
  int transaction_nesting() const {  
    return db_.transaction_nesting();
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool RecreateAllTablesButURL();

  
  
  void Vacuum();

  
  
  
  
  
  
  
  bool needs_version_17_migration() const {
    return needs_version_17_migration_;
  }

  
  void ThumbnailMigrationDone();

  
  bool GetNeedsThumbnailMigration();

  

  
  bool SetSegmentID(VisitID visit_id, SegmentID segment_id);

  
  
  SegmentID GetSegmentID(VisitID visit_id);

  
  
  
  virtual base::Time GetEarlyExpirationThreshold();
  virtual void UpdateEarlyExpirationThreshold(base::Time threshold);

 private:
  FRIEND_TEST_ALL_PREFIXES(IconMappingMigrationTest, TestIconMappingMigration);
  
  virtual sql::Connection& GetDB();

  

  
  
  
  
  
  
  
  sql::InitStatus EnsureCurrentVersion(const FilePath& tmp_bookmarks_path);

#if !defined(OS_WIN)
  
  
  void MigrateTimeEpoch();
#endif

  

  sql::Connection db_;
  sql::MetaTable meta_table_;

  base::Time cached_early_expiration_threshold_;

  
  bool needs_version_17_migration_;

  DISALLOW_COPY_AND_ASSIGN(HistoryDatabase);
};

}  

#endif  
