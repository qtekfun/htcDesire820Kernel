// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOM_STORAGE_SESSION_STORAGE_DATABASE_H_
#define CONTENT_BROWSER_DOM_STORAGE_SESSION_STORAGE_DATABASE_H_

#include <map>
#include <string>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "content/common/content_export.h"
#include "content/common/dom_storage/dom_storage_types.h"
#include "third_party/leveldatabase/src/include/leveldb/status.h"

class GURL;

namespace leveldb {
class DB;
struct ReadOptions;
class WriteBatch;
}  

namespace content {


class CONTENT_EXPORT SessionStorageDatabase :
    public base::RefCountedThreadSafe<SessionStorageDatabase> {
 public:
  explicit SessionStorageDatabase(const base::FilePath& file_path);

  
  // assumed to be empty and any duplicate keys will be overwritten. If the
  
  
  void ReadAreaValues(const std::string& namespace_id,
                      const GURL& origin,
                      DOMStorageValuesMap* result);

  
  
  
  
  
  
  bool CommitAreaChanges(const std::string& namespace_id,
                         const GURL& origin,
                         bool clear_all_first,
                         const DOMStorageValuesMap& changes);

  
  
  bool CloneNamespace(const std::string& namespace_id,
                      const std::string& new_namespace_id);

  
  bool DeleteArea(const std::string& namespace_id, const GURL& origin);

  
  bool DeleteNamespace(const std::string& namespace_id);

  
  bool ReadNamespacesAndOrigins(
      std::map<std::string, std::vector<GURL> >* namespaces_and_origins);

 private:
  friend class base::RefCountedThreadSafe<SessionStorageDatabase>;
  friend class SessionStorageDatabaseTest;

  ~SessionStorageDatabase();

  
  
  
  
  
  
  bool LazyOpen(bool create_if_needed);

  
  
  leveldb::Status TryToOpen(leveldb::DB** db);

  
  bool IsOpen() const;

  
  
  bool CallerErrorCheck(bool ok) const;
  bool ConsistencyCheck(bool ok);
  bool DatabaseErrorCheck(bool ok);

  
  
  
  

  
  
  
  bool CreateNamespace(const std::string& namespace_id,
                       bool ok_if_exists,
                       leveldb::WriteBatch* batch);

  
  
  bool GetAreasInNamespace(const std::string& namespace_id,
                           std::map<std::string, std::string>* areas);

  
  
  void AddAreaToNamespace(const std::string& namespace_id,
                          const std::string& origin,
                          const std::string& map_id,
                          leveldb::WriteBatch* batch);

  
  bool DeleteAreaHelper(const std::string& namespace_id,
                        const std::string& origin,
                        leveldb::WriteBatch* batch);

  
  
  bool GetMapForArea(const std::string& namespace_id,
                     const std::string& origin,
                     const leveldb::ReadOptions& options,
                     bool* exists,
                     std::string* map_id);

  
  
  
  
  bool CreateMapForArea(const std::string& namespace_id,
                        const GURL& origin,
                        std::string* map_id,
                        leveldb::WriteBatch* batch);
  
  
  
  bool ReadMap(const std::string& map_id,
               const leveldb::ReadOptions& options,
               DOMStorageValuesMap* result,
               bool only_keys);
  
  void WriteValuesToMap(const std::string& map_id,
                        const DOMStorageValuesMap& values,
                        leveldb::WriteBatch* batch);

  bool GetMapRefCount(const std::string& map_id, int64* ref_count);
  bool IncreaseMapRefCount(const std::string& map_id,
                           leveldb::WriteBatch* batch);
  
  
  bool DecreaseMapRefCount(const std::string& map_id,
                           int decrease,
                           leveldb::WriteBatch* batch);

  
  bool ClearMap(const std::string& map_id, leveldb::WriteBatch* batch);

  
  
  
  bool DeepCopyArea(const std::string& namespace_id,
                    const GURL& origin,
                    bool copy_data,
                    std::string* map_id,
                    leveldb::WriteBatch* batch);

  
  static std::string NamespaceStartKey(const std::string& namespace_id);
  static std::string NamespaceKey(const std::string& namespace_id,
                                  const std::string& origin);
  static const char* NamespacePrefix();
  static std::string MapRefCountKey(const std::string& map_id);
  static std::string MapKey(const std::string& map_id, const std::string& key);
  static const char* NextMapIdKey();

  scoped_ptr<leveldb::DB> db_;
  base::FilePath file_path_;

  
  base::Lock db_lock_;

  
  bool db_error_;
  
  bool is_inconsistent_;

  DISALLOW_COPY_AND_ASSIGN(SessionStorageDatabase);
};

}  

#endif  
