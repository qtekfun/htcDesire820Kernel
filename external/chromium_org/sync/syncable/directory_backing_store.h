// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SYNCABLE_DIRECTORY_BACKING_STORE_H_
#define SYNC_SYNCABLE_DIRECTORY_BACKING_STORE_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "base/threading/non_thread_safe.h"
#include "sql/connection.h"
#include "sql/statement.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/syncable/dir_open_result.h"
#include "sync/syncable/directory.h"
#include "sync/syncable/metahandle_set.h"

namespace sync_pb {
class EntitySpecifics;
}

namespace syncer {
namespace syncable {

SYNC_EXPORT_PRIVATE extern const int32 kCurrentDBVersion;

struct ColumnSpec;

class SYNC_EXPORT_PRIVATE DirectoryBackingStore : public base::NonThreadSafe {
 public:
  explicit DirectoryBackingStore(const std::string& dir_name);
  virtual ~DirectoryBackingStore();

  
  
  
  
  
  
  
  
  
  virtual DirOpenResult Load(Directory::MetahandlesMap* handles_map,
                             JournalIndex* delete_journals,
                             Directory::KernelLoadInfo* kernel_load_info) = 0;

  
  
  
  
  
  virtual bool SaveChanges(const Directory::SaveChangesSnapshot& snapshot);

 protected:
  
  DirectoryBackingStore(const std::string& dir_name,
                        sql::Connection* connection);

  
  bool InitializeTables();
  bool CreateTables();

  
  
  bool CreateShareInfoTable(bool is_temporary);

  bool CreateShareInfoTableVersion71(bool is_temporary);
  
  
  bool CreateMetasTable(bool is_temporary);
  bool CreateModelsTable();
  bool CreateV71ModelsTable();
  bool CreateV75ModelsTable();

  
  
  bool DropDeletedEntries();
  
  bool SafeDropTable(const char* table_name);

  
  bool LoadEntries(Directory::MetahandlesMap* handles_map);
  bool LoadDeleteJournals(JournalIndex* delete_journals);
  bool LoadInfo(Directory::KernelLoadInfo* info);

  
  static bool SaveEntryToDB(sql::Statement* save_statement,
                            const EntryKernel& entry);
  bool SaveNewEntryToDB(const EntryKernel& entry);
  bool UpdateEntryToDB(const EntryKernel& entry);

  
  void EndSave();

  enum EntryTable {
    METAS_TABLE,
    DELETE_JOURNAL_TABLE,
  };
  
  
  bool DeleteEntries(EntryTable from, const MetahandleSet& handles);

  
  void DropAllTables();

  
  
  
  
  static ModelType ModelIdToModelTypeEnum(const void* data, int length);
  static std::string ModelTypeEnumToModelId(ModelType model_type);

  static std::string GenerateCacheGUID();

  
  
  
  bool CheckIntegrity(sqlite3* handle, std::string* error) const;

  
  static bool VerifyReferenceIntegrity(
      const Directory::MetahandlesMap* handles_map);

  
  bool RefreshColumns();
  bool SetVersion(int version);
  int GetVersion();

  bool MigrateToSpecifics(const char* old_columns,
                          const char* specifics_column,
                          void(*handler_function) (
                              sql::Statement* old_value_query,
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
  bool MigrateVersion75To76();
  bool MigrateVersion76To77();
  bool MigrateVersion77To78();
  bool MigrateVersion78To79();
  bool MigrateVersion79To80();
  bool MigrateVersion80To81();
  bool MigrateVersion81To82();
  bool MigrateVersion82To83();
  bool MigrateVersion83To84();
  bool MigrateVersion84To85();
  bool MigrateVersion85To86();

  scoped_ptr<sql::Connection> db_;
  sql::Statement save_meta_statment_;
  sql::Statement save_delete_journal_statment_;
  std::string dir_name_;

  
  
  bool needs_column_refresh_;

 private:
  
  void PrepareSaveEntryStatement(EntryTable table,
                                 sql::Statement* save_statement);

  DISALLOW_COPY_AND_ASSIGN(DirectoryBackingStore);
};

}  
}  

#endif  
