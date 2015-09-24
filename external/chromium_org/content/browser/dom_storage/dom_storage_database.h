// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOM_STORAGE_DOM_STORAGE_DATABASE_H_
#define CONTENT_BROWSER_DOM_STORAGE_DOM_STORAGE_DATABASE_H_

#include <map>

#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/nullable_string16.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "content/common/dom_storage/dom_storage_types.h"
#include "sql/connection.h"

namespace content {

class CONTENT_EXPORT DOMStorageDatabase {
 public:
  static base::FilePath GetJournalFilePath(const base::FilePath& database_path);

  explicit DOMStorageDatabase(const base::FilePath& file_path);
  virtual ~DOMStorageDatabase();  

  
  
  // be overwritten. If the database exists on disk then it will be
  
  
  void ReadAllValues(DOMStorageValuesMap* result);

  
  
  
  
  bool CommitChanges(bool clear_all_first, const DOMStorageValuesMap& changes);

  
  const base::FilePath& file_path() const { return file_path_; }

 protected:
  
  DOMStorageDatabase();

 private:
  friend class LocalStorageDatabaseAdapter;
  FRIEND_TEST_ALL_PREFIXES(DOMStorageDatabaseTest, SimpleOpenAndClose);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageDatabaseTest, TestLazyOpenIsLazy);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageDatabaseTest, TestDetectSchemaVersion);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageDatabaseTest,
                           TestLazyOpenUpgradesDatabase);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageDatabaseTest, SimpleWriteAndReadBack);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageDatabaseTest, WriteWithClear);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageDatabaseTest,
                           UpgradeFromV1ToV2WithData);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageDatabaseTest, TestSimpleRemoveOneValue);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageDatabaseTest,
                           TestCanOpenAndReadWebCoreDatabase);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageDatabaseTest,
                           TestCanOpenFileThatIsNotADatabase);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageAreaTest, BackingDatabaseOpened);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageAreaTest, CommitTasks);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageAreaTest, PurgeMemory);

  enum SchemaVersion {
    INVALID,
    V1,
    V2
  };

  
  
  
  
  bool LazyOpen(bool create_if_needed);

  
  
  SchemaVersion DetectSchemaVersion();

  
  
  
  bool CreateTableV2();

  
  
  
  
  bool DeleteFileAndRecreate();

  
  
  
  
  bool UpgradeVersion1To2();

  void Close();
  bool IsOpen() const { return db_.get() ? db_->is_open() : false; }

  
  void Init();

  
  const base::FilePath file_path_;
  scoped_ptr<sql::Connection> db_;
  bool failed_to_open_;
  bool tried_to_recreate_;
  bool known_to_be_empty_;
};

}  

#endif  
