// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DOM_STORAGE_DOM_STORAGE_CONTEXT_IMPL_H_
#define CONTENT_BROWSER_DOM_STORAGE_DOM_STORAGE_CONTEXT_IMPL_H_

#include <map>
#include <set>
#include <vector>

#include "base/atomic_sequence_num.h"
#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "base/observer_list.h"
#include "base/time/time.h"
#include "content/browser/dom_storage/dom_storage_namespace.h"
#include "content/common/content_export.h"
#include "content/public/browser/session_storage_namespace.h"
#include "url/gurl.h"

namespace base {
class FilePath;
class NullableString16;
class Time;
}

namespace quota {
class SpecialStoragePolicy;
}

namespace content {

class DOMStorageArea;
class DOMStorageSession;
class DOMStorageTaskRunner;
class SessionStorageDatabase;
struct LocalStorageUsageInfo;
struct SessionStorageUsageInfo;

class CONTENT_EXPORT DOMStorageContextImpl
    : public base::RefCountedThreadSafe<DOMStorageContextImpl> {
 public:
  
  
  class EventObserver {
   public:
    
    virtual void OnDOMStorageItemSet(
        const DOMStorageArea* area,
        const base::string16& key,
        const base::string16& new_value,
        const base::NullableString16& old_value,
        const GURL& page_url) = 0;
    virtual void OnDOMStorageItemRemoved(
        const DOMStorageArea* area,
        const base::string16& key,
        const base::string16& old_value,
        const GURL& page_url) = 0;
    virtual void OnDOMStorageAreaCleared(
        const DOMStorageArea* area,
        const GURL& page_url) = 0;
    
    
    virtual void OnDOMSessionStorageReset(int64 namespace_id) = 0;

   protected:
    virtual ~EventObserver() {}
  };

  
  
  DOMStorageContextImpl(
      const base::FilePath& localstorage_directory,
      const base::FilePath& sessionstorage_directory,
      quota::SpecialStoragePolicy* special_storage_policy,
      DOMStorageTaskRunner* task_runner);

  
  
  const base::FilePath& localstorage_directory() {
    return localstorage_directory_;
  }

  
  
  const base::FilePath& sessionstorage_directory() {
    return sessionstorage_directory_;
  }

  DOMStorageTaskRunner* task_runner() const { return task_runner_.get(); }
  DOMStorageNamespace* GetStorageNamespace(int64 namespace_id);

  void GetLocalStorageUsage(std::vector<LocalStorageUsageInfo>* infos,
                            bool include_file_info);
  void GetSessionStorageUsage(std::vector<SessionStorageUsageInfo>* infos);
  void DeleteLocalStorage(const GURL& origin);
  void DeleteSessionStorage(const SessionStorageUsageInfo& usage_info);
  void PurgeMemory();

  
  
  
  
  void SetForceKeepSessionState() {
    force_keep_session_state_ = true;
  }

  
  
  
  
  
  void Shutdown();

  
  void AddEventObserver(EventObserver* observer);
  void RemoveEventObserver(EventObserver* observer);

  void NotifyItemSet(
      const DOMStorageArea* area,
      const base::string16& key,
      const base::string16& new_value,
      const base::NullableString16& old_value,
      const GURL& page_url);
  void NotifyItemRemoved(
      const DOMStorageArea* area,
      const base::string16& key,
      const base::string16& old_value,
      const GURL& page_url);
  void NotifyAreaCleared(
      const DOMStorageArea* area,
      const GURL& page_url);
  void NotifyAliasSessionMerged(
      int64 namespace_id,
      DOMStorageNamespace* old_alias_master_namespace);

  
  int64 AllocateSessionId() {
    return session_id_sequence_.GetNext();
  }

  std::string AllocatePersistentSessionId();

  
  void CreateSessionNamespace(int64 namespace_id,
                              const std::string& persistent_namespace_id);
  void DeleteSessionNamespace(int64 namespace_id, bool should_persist_data);
  void CloneSessionNamespace(int64 existing_id, int64 new_id,
                             const std::string& new_persistent_id);
  void CreateAliasSessionNamespace(int64 existing_id, int64 new_id,
                                   const std::string& persistent_id);

  
  
  void SetSaveSessionStorageOnDisk();

  
  
  
  void StartScavengingUnusedSessionStorage();

  void AddTransactionLogProcessId(int64 namespace_id, int process_id);
  void RemoveTransactionLogProcessId(int64 namespace_id, int process_id);

  SessionStorageNamespace::MergeResult MergeSessionStorage(
      int64 namespace1_id, bool actually_merge, int process_id,
      int64 namespace2_id);

 private:
  friend class DOMStorageContextImplTest;
  FRIEND_TEST_ALL_PREFIXES(DOMStorageContextImplTest, Basics);
  friend class base::RefCountedThreadSafe<DOMStorageContextImpl>;
  typedef std::map<int64, scoped_refptr<DOMStorageNamespace> >
      StorageNamespaceMap;

  virtual ~DOMStorageContextImpl();

  void ClearSessionOnlyOrigins();

  void MaybeShutdownSessionNamespace(DOMStorageNamespace* ns);

  
  void FindUnusedNamespaces();
  void FindUnusedNamespacesInCommitSequence(
      const std::set<std::string>& namespace_ids_in_use,
      const std::set<std::string>& protected_persistent_session_ids);
  void DeleteNextUnusedNamespace();
  void DeleteNextUnusedNamespaceInCommitSequence();

  
  StorageNamespaceMap namespaces_;

  
  base::FilePath localstorage_directory_;

  
  
  
  base::FilePath sessionstorage_directory_;

  
  scoped_refptr<DOMStorageTaskRunner> task_runner_;

  
  ObserverList<EventObserver> event_observers_;

  
  
  base::AtomicSequenceNumber session_id_sequence_;

  bool is_shutdown_;
  bool force_keep_session_state_;
  scoped_refptr<quota::SpecialStoragePolicy> special_storage_policy_;
  scoped_refptr<SessionStorageDatabase> session_storage_database_;

  
  bool scavenging_started_;
  std::vector<std::string> deletable_persistent_namespace_ids_;

  
  
  std::set<std::string> protected_persistent_session_ids_;

  
  
  std::map<std::string, int64> persistent_namespace_id_to_namespace_id_;
};

}  

#endif  
