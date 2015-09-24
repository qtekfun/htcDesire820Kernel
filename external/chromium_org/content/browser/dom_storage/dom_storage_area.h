// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOM_STORAGE_DOM_STORAGE_AREA_H_
#define CONTENT_BROWSER_DOM_STORAGE_DOM_STORAGE_AREA_H_

#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/nullable_string16.h"
#include "base/strings/string16.h"
#include "content/common/content_export.h"
#include "content/common/dom_storage/dom_storage_types.h"
#include "url/gurl.h"

namespace content {

class DOMStorageDatabaseAdapter;
class DOMStorageMap;
class DOMStorageTaskRunner;
class SessionStorageDatabase;

class CONTENT_EXPORT DOMStorageArea
    : public base::RefCountedThreadSafe<DOMStorageArea> {

 public:
  static const base::FilePath::CharType kDatabaseFileExtension[];
  static base::FilePath DatabaseFileNameFromOrigin(const GURL& origin);
  static GURL OriginFromDatabaseFileName(const base::FilePath& file_name);

  
  DOMStorageArea(const GURL& origin,
                 const base::FilePath& directory,
                 DOMStorageTaskRunner* task_runner);

  
  DOMStorageArea(int64 namespace_id,
                 const std::string& persistent_namespace_id,
                 const GURL& origin,
                 SessionStorageDatabase* session_storage_backing,
                 DOMStorageTaskRunner* task_runner);

  const GURL& origin() const { return origin_; }
  int64 namespace_id() const { return namespace_id_; }

  
  void ExtractValues(DOMStorageValuesMap* map);

  unsigned Length();
  base::NullableString16 Key(unsigned index);
  base::NullableString16 GetItem(const base::string16& key);
  bool SetItem(const base::string16& key, const base::string16& value,
               base::NullableString16* old_value);
  bool RemoveItem(const base::string16& key, base::string16* old_value);
  bool Clear();
  void FastClear();

  DOMStorageArea* ShallowCopy(
      int64 destination_namespace_id,
      const std::string& destination_persistent_namespace_id);

  bool HasUncommittedChanges() const;

  
  
  void DeleteOrigin();

  
  
  
  void PurgeMemory();

  
  
  
  void Shutdown();

  
  bool IsLoadedInMemory() const { return is_initial_import_done_; }

 private:
  friend class DOMStorageAreaTest;
  FRIEND_TEST_ALL_PREFIXES(DOMStorageAreaTest, DOMStorageAreaBasics);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageAreaTest, BackingDatabaseOpened);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageAreaTest, TestDatabaseFilePath);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageAreaTest, CommitTasks);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageAreaTest, CommitChangesAtShutdown);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageAreaTest, DeleteOrigin);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageAreaTest, PurgeMemory);
  FRIEND_TEST_ALL_PREFIXES(DOMStorageContextImplTest, PersistentIds);
  friend class base::RefCountedThreadSafe<DOMStorageArea>;

  struct CommitBatch {
    bool clear_all_first;
    DOMStorageValuesMap changed_values;
    CommitBatch();
    ~CommitBatch();
  };

  ~DOMStorageArea();

  
  
  
  void InitialImportIfNeeded();

  
  
  
  CommitBatch* CreateCommitBatchIfNeeded();
  void OnCommitTimer();
  void CommitChanges(const CommitBatch* commit_batch);
  void OnCommitComplete();

  void ShutdownInCommitSequence();

  int64 namespace_id_;
  std::string persistent_namespace_id_;
  GURL origin_;
  base::FilePath directory_;
  scoped_refptr<DOMStorageTaskRunner> task_runner_;
  scoped_refptr<DOMStorageMap> map_;
  scoped_ptr<DOMStorageDatabaseAdapter> backing_;
  scoped_refptr<SessionStorageDatabase> session_storage_backing_;
  bool is_initial_import_done_;
  bool is_shutdown_;
  scoped_ptr<CommitBatch> commit_batch_;
  int commit_batches_in_flight_;
};

}  

#endif  
