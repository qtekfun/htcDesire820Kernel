// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOM_STORAGE_DOM_STORAGE_NAMESPACE_H_
#define CONTENT_BROWSER_DOM_STORAGE_DOM_STORAGE_NAMESPACE_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/strings/nullable_string16.h"
#include "content/common/content_export.h"
#include "content/public/browser/session_storage_namespace.h"
#include "url/gurl.h"

namespace content {

class DOMStorageArea;
class DOMStorageContextImpl;
class DOMStorageTaskRunner;
class SessionStorageDatabase;

class CONTENT_EXPORT DOMStorageNamespace
    : public base::RefCountedThreadSafe<DOMStorageNamespace> {
 public:
  
  enum PurgeOption {
    
    PURGE_UNOPENED,

    
    
    PURGE_AGGRESSIVE,
  };

  
  
  DOMStorageNamespace(const base::FilePath& directory,  
                      DOMStorageTaskRunner* task_runner);

  
  
  DOMStorageNamespace(int64 namespace_id,
                      const std::string& persistent_namespace_id,
                      SessionStorageDatabase* session_storage_database,
                      DOMStorageTaskRunner* task_runner);

  int64 namespace_id() const { return namespace_id_; }
  const std::string& persistent_namespace_id() const {
    return persistent_namespace_id_;
  }

  
  
  
  DOMStorageArea* OpenStorageArea(const GURL& origin);
  void CloseStorageArea(DOMStorageArea* area);

  
  DOMStorageArea* GetOpenStorageArea(const GURL& origin);

  
  
  
  DOMStorageNamespace* Clone(int64 clone_namespace_id,
                             const std::string& clone_persistent_namespace_id);

  
  
  DOMStorageNamespace* CreateAlias(int64 alias_namespace_id);

  void DeleteLocalStorageOrigin(const GURL& origin);
  void DeleteSessionStorageOrigin(const GURL& origin);
  void PurgeMemory(PurgeOption purge);
  void Shutdown();

  unsigned int CountInMemoryAreas() const;

  void AddTransactionLogProcessId(int process_id);
  void RemoveTransactionLogProcessId(int process_id);
  SessionStorageNamespace::MergeResult Merge(
      bool actually_merge,
      int process_id,
      DOMStorageNamespace* other,
      DOMStorageContextImpl* context);
  DOMStorageNamespace* alias_master_namespace() {
    return alias_master_namespace_.get();
  }
  int num_aliases() const { return num_aliases_; }
  bool ready_for_deletion_pending_aliases() const {
    return ready_for_deletion_pending_aliases_; }
  void set_ready_for_deletion_pending_aliases(bool value) {
    ready_for_deletion_pending_aliases_ = value;
  }
  bool must_persist_at_shutdown() const { return must_persist_at_shutdown_; }
  void set_must_persist_at_shutdown(bool value) {
    must_persist_at_shutdown_ = value;
  }

  enum LogType {
    TRANSACTION_READ,
    TRANSACTION_WRITE,
    TRANSACTION_REMOVE,
    TRANSACTION_CLEAR
  };

  struct CONTENT_EXPORT TransactionRecord {
    LogType transaction_type;
    GURL origin;
    GURL page_url;
    base::string16 key;
    base::NullableString16 value;
    TransactionRecord();
    ~TransactionRecord();
  };

  void AddTransaction(int process_id, const TransactionRecord& transaction);
  bool IsLoggingRenderer(int process_id);
  
  
  bool DecrementMasterAliasCount();

 private:
  friend class base::RefCountedThreadSafe<DOMStorageNamespace>;

  
  
  struct AreaHolder {
    scoped_refptr<DOMStorageArea> area_;
    int open_count_;
    AreaHolder();
    AreaHolder(DOMStorageArea* area, int count);
    ~AreaHolder();
  };
  typedef std::map<GURL, AreaHolder> AreaMap;

  struct TransactionData {
    bool max_log_size_exceeded;
    std::vector<TransactionRecord> log;
    TransactionData();
    ~TransactionData();
  };

  ~DOMStorageNamespace();

  
  AreaHolder* GetAreaHolder(const GURL& origin);

  
  void SwitchToNewAliasMaster(DOMStorageNamespace* new_master,
                              DOMStorageContextImpl* context);

  int64 namespace_id_;
  std::string persistent_namespace_id_;
  base::FilePath directory_;
  AreaMap areas_;
  scoped_refptr<DOMStorageTaskRunner> task_runner_;
  scoped_refptr<SessionStorageDatabase> session_storage_database_;
  std::map<int, TransactionData*> transactions_;
  int num_aliases_;
  scoped_refptr<DOMStorageNamespace> alias_master_namespace_;
  DOMStorageNamespace* old_master_for_close_area_;
  
  
  
  
  
  bool master_alias_count_decremented_;
  
  
  
  bool ready_for_deletion_pending_aliases_;
  bool must_persist_at_shutdown_;
};

}  


#endif  
