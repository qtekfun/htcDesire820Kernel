// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SYNCABLE_DIRECTORY_BACKING_STORE_H_
#define CHROME_BROWSER_SYNC_SYNCABLE_DIRECTORY_BACKING_STORE_H_
#pragma once

#include <string>

#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "chrome/browser/sync/syncable/dir_open_result.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "chrome/browser/sync/syncable/syncable.h"

extern "C" {
struct sqlite3;
struct sqlite3_stmt;
}

class SQLStatement;

namespace sync_pb {
class EntitySpecifics;
}

namespace syncable {

struct ColumnSpec;
typedef Directory::MetahandlesIndex MetahandlesIndex;

class DirectoryBackingStore {
 public:
  DirectoryBackingStore(const std::string& dir_name,
                        const FilePath& backing_filepath);

  virtual ~DirectoryBackingStore();

  
  
  
  
  DirOpenResult Load(MetahandlesIndex* entry_bucket,
                     Directory::KernelLoadInfo* kernel_load_info);

  
  
  
  
  
  virtual bool SaveChanges(const Directory::SaveChangesSnapshot& snapshot);

 private:
  FRIEND_TEST_ALL_PREFIXES(DirectoryBackingStoreTest, MigrateVersion67To68);
  FRIEND_TEST_ALL_PREFIXES(DirectoryBackingStoreTest, MigrateVersion68To69);
  FRIEND_TEST_ALL_PREFIXES(DirectoryBackingStoreTest, MigrateVersion69To70);
  FRIEND_TEST_ALL_PREFIXES(DirectoryBackingStoreTest, MigrateVersion70To71);
  FRIEND_TEST_ALL_PREFIXES(DirectoryBackingStoreTest, MigrateVersion71To72);
  FRIEND_TEST_ALL_PREFIXES(DirectoryBackingStoreTest, MigrateVersion72To73);
  FRIEND_TEST_ALL_PREFIXES(DirectoryBackingStoreTest, MigrateVersion73To74);
  FRIEND_TEST_ALL_PREFIXES(DirectoryBackingStoreTest, MigrateVersion74To75);
  FRIEND_TEST_ALL_PREFIXES(DirectoryBackingStoreTest, ModelTypeIds);
  FRIEND_TEST_ALL_PREFIXES(DirectoryBackingStoreTest, Corruption);
  FRIEND_TEST_ALL_PREFIXES(DirectoryBackingStoreTest, DeleteEntries);
  FRIEND_TEST_ALL_PREFIXES(MigrationTest, ToCurrentVersion);
  friend class MigrationTest;

  
  DirOpenResult InitializeTables();
  
  int CreateTables();

  
  
  
  int CreateShareInfoTable(bool is_temporary);

  int CreateShareInfoTableVersion71(bool is_temporary);
  
  
  int CreateMetasTable(bool is_temporary);
  
  int CreateModelsTable();
  int CreateV71ModelsTable();

  
  
  bool DropDeletedEntries();
  
  int SafeDropTable(const char* table_name);

  
  bool LoadEntries(MetahandlesIndex* entry_bucket);
  bool LoadInfo(Directory::KernelLoadInfo* info);

  
  bool SaveEntryToDB(const EntryKernel& entry);
  bool SaveNewEntryToDB(const EntryKernel& entry);
  bool UpdateEntryToDB(const EntryKernel& entry);

  
  
  
  
  bool OpenAndConfigureHandleHelper(sqlite3** handle) const;
  
  bool BeginLoad();
  void EndLoad();
  DirOpenResult DoLoad(MetahandlesIndex* entry_bucket,
      Directory::KernelLoadInfo* kernel_load_info);

  
  void EndSave();

  
  
  bool DeleteEntries(const MetahandleSet& handles);

  
  sqlite3* LazyGetSaveHandle();

  
  void DropAllTables();

  
  
  
  
  static ModelType ModelIdToModelTypeEnum(const void* data, int length);
  static std::string ModelTypeEnumToModelId(ModelType model_type);

  
  
  
  bool CheckIntegrity(sqlite3* handle, std::string* error) const;

  
  bool AddColumn(const ColumnSpec* column);
  bool RefreshColumns();
  bool SetVersion(int version);
  int GetVersion();
  bool MigrateToSpecifics(const char* old_columns,
                          const char* specifics_column,
                          void(*handler_function) (
                              SQLStatement* old_value_query,
                              int old_value_column,
                              sync_pb::EntitySpecifics* mutable_new_value));

  
  bool MigrateVersion67To68();
  bool MigrateVersion68To69();
  bool MigrateVersion69To70();
  bool MigrateVersion70To71();
  bool MigrateVersion71To72();
  bool MigrateVersion72To73();
  bool MigrateVersion73To74();
  bool MigrateVersion74To75();

  
  
  
  
  
  
  sqlite3* load_dbhandle_;
  sqlite3* save_dbhandle_;

  std::string dir_name_;
  FilePath backing_filepath_;

  
  
  bool needs_column_refresh_;

  DISALLOW_COPY_AND_ASSIGN(DirectoryBackingStore);
};

}  

#endif  
