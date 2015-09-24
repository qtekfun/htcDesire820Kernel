// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SYNCABLE_DIRECTORY_H_
#define SYNC_SYNCABLE_DIRECTORY_H_

#include <deque>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/file_util.h"
#include "base/gtest_prod_util.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/util/report_unrecoverable_error_function.h"
#include "sync/internal_api/public/util/weak_handle.h"
#include "sync/syncable/dir_open_result.h"
#include "sync/syncable/entry_kernel.h"
#include "sync/syncable/metahandle_set.h"
#include "sync/syncable/parent_child_index.h"
#include "sync/syncable/syncable_delete_journal.h"

namespace syncer {

class Cryptographer;
class TestUserShare;
class UnrecoverableErrorHandler;

namespace syncable {

class BaseTransaction;
class BaseWriteTransaction;
class DirectoryChangeDelegate;
class DirectoryBackingStore;
class NigoriHandler;
class ScopedKernelLock;
class TransactionObserver;
class WriteTransaction;

enum InvariantCheckLevel {
  OFF = 0,            
  VERIFY_CHANGES = 1, 
  FULL_DB_VERIFICATION = 2 
};

class SYNC_EXPORT Directory {
  friend class BaseTransaction;
  friend class Entry;
  friend class ModelNeutralMutableEntry;
  friend class MutableEntry;
  friend class ReadTransaction;
  friend class ScopedKernelLock;
  friend class WriteTransaction;
  friend class SyncableDirectoryTest;
  friend class syncer::TestUserShare;
  FRIEND_TEST_ALL_PREFIXES(SyncableDirectoryTest, ManageDeleteJournals);
  FRIEND_TEST_ALL_PREFIXES(SyncableDirectoryTest,
                           TakeSnapshotGetsAllDirtyHandlesTest);
  FRIEND_TEST_ALL_PREFIXES(SyncableDirectoryTest,
                           TakeSnapshotGetsOnlyDirtyHandlesTest);
  FRIEND_TEST_ALL_PREFIXES(SyncableDirectoryTest,
                           TakeSnapshotGetsMetahandlesToPurge);

 public:
  typedef std::vector<int64> Metahandles;

  
  
  
  
  
  
  
  
  
  typedef base::hash_map<int64, EntryKernel*> MetahandlesMap;
  typedef base::hash_map<std::string, EntryKernel*> IdsMap;
  typedef base::hash_map<std::string, EntryKernel*> TagsMap;

  static const base::FilePath::CharType kSyncDatabaseFilename[];

  
  
  enum KernelShareInfoStatus {
    KERNEL_SHARE_INFO_INVALID,
    KERNEL_SHARE_INFO_VALID,
    KERNEL_SHARE_INFO_DIRTY
  };

  
  
  struct SYNC_EXPORT_PRIVATE PersistedKernelInfo {
    PersistedKernelInfo();
    ~PersistedKernelInfo();

    
    
    
    void reset_download_progress(ModelType model_type);

    
    sync_pb::DataTypeProgressMarker download_progress[MODEL_TYPE_COUNT];
    
    
    
    
    
    
    int64 transaction_version[MODEL_TYPE_COUNT];
    
    
    std::string store_birthday;
    
    int64 next_id;
    
    
    
    std::string bag_of_chips;
  };

  
  
  struct KernelLoadInfo {
    PersistedKernelInfo kernel_info;
    std::string cache_guid;  
    int64 max_metahandle;    
    KernelLoadInfo() : max_metahandle(0) {
    }
  };

  
  
  
  struct SYNC_EXPORT_PRIVATE SaveChangesSnapshot {
    SaveChangesSnapshot();
    ~SaveChangesSnapshot();

    KernelShareInfoStatus kernel_info_status;
    PersistedKernelInfo kernel_info;
    EntryKernelSet dirty_metas;
    MetahandleSet metahandles_to_purge;
    EntryKernelSet delete_journals;
    MetahandleSet delete_journals_to_purge;
  };

  
  
  
  Directory(
      DirectoryBackingStore* store,
      UnrecoverableErrorHandler* unrecoverable_error_handler,
      ReportUnrecoverableErrorFunction
          report_unrecoverable_error_function,
      NigoriHandler* nigori_handler,
      Cryptographer* cryptographer);
  virtual ~Directory();

  
  
  
  
  DirOpenResult Open(const std::string& name,
                     DirectoryChangeDelegate* delegate,
                     const WeakHandle<TransactionObserver>&
                         transaction_observer);

  
  
  void Close();

  int64 NextMetahandle();
  
  syncable::Id NextId();

  bool good() const { return NULL != kernel_; }

  
  
  void GetDownloadProgress(
      ModelType type,
      sync_pb::DataTypeProgressMarker* value_out) const;
  void GetDownloadProgressAsString(
      ModelType type,
      std::string* value_out) const;
  size_t GetEntriesCount() const;
  void SetDownloadProgress(
      ModelType type,
      const sync_pb::DataTypeProgressMarker& value);

  
  
  int64 GetTransactionVersion(ModelType type) const;
  void IncrementTransactionVersion(ModelType type);

  ModelTypeSet InitialSyncEndedTypes();
  bool InitialSyncEndedForType(ModelType type);
  bool InitialSyncEndedForType(BaseTransaction* trans, ModelType type);

  const std::string& name() const { return kernel_->name; }

  
  
  
  std::string store_birthday() const;
  void set_store_birthday(const std::string& store_birthday);

  
  
  std::string bag_of_chips() const;
  void set_bag_of_chips(const std::string& bag_of_chips);

  
  std::string cache_guid() const;

  
  NigoriHandler* GetNigoriHandler();

  
  
  Cryptographer* GetCryptographer(const BaseTransaction* trans);

  
  
  
  
  bool unrecoverable_error_set(const BaseTransaction* trans) const;

  
  
  void ReportUnrecoverableError() {
    if (report_unrecoverable_error_function_) {
      report_unrecoverable_error_function_();
    }
  }

  
  
  void OnUnrecoverableError(const BaseTransaction* trans,
                            const tracked_objects::Location& location,
                            const std::string & message);

  DeleteJournal* delete_journal();

  
  
  
  bool GetChildHandlesById(BaseTransaction*, const Id& parent_id,
      Metahandles* result);

  
  
  
  bool GetChildHandlesByHandle(BaseTransaction*, int64 handle,
      Metahandles* result);

  
  int GetTotalNodeCount(BaseTransaction*, EntryKernel* kernel_) const;

  
  
  int GetPositionIndex(BaseTransaction*, EntryKernel* kernel_) const;

  
  bool HasChildren(BaseTransaction* trans, const Id& id);

  
  
  
  
  Id GetFirstChildId(BaseTransaction* trans, const EntryKernel* parent);

  
  
  
  
  
  
  syncable::Id GetPredecessorId(EntryKernel*);
  syncable::Id GetSuccessorId(EntryKernel*);

  
  
  
  
  
  
  
  
  
  void PutPredecessor(EntryKernel* e, EntryKernel* predecessor);

  
  
  
  
  
  
  
  
  bool SaveChanges();

  
  void GetAllEntryKernels(BaseTransaction* trans,
                          std::vector<const EntryKernel*>* result);

  
  int64 unsynced_entity_count() const;

  
  
  
  void GetUnsyncedMetaHandles(BaseTransaction* trans,
                              Metahandles* result);

  
  bool TypeHasUnappliedUpdates(ModelType type);

  
  
  void GetUnappliedUpdateMetaHandles(BaseTransaction* trans,
                                     FullModelTypeSet server_types,
                                     std::vector<int64>* result);

  
  
  
  
  void CollectMetaHandleCounts(std::vector<int>* num_entries_by_type,
                               std::vector<int>* num_to_delete_entries_by_type);

  
  void SetInvariantCheckLevel(InvariantCheckLevel check_level);

  
  
  
  bool CheckInvariantsOnTransactionClose(
      syncable::BaseTransaction* trans,
      const MetahandleSet& modified_handles);

  
  
  bool FullyCheckTreeInvariants(BaseTransaction *trans);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool PurgeEntriesWithTypeIn(ModelTypeSet disabled_types,
                                      ModelTypeSet types_to_journal,
                                      ModelTypeSet types_to_unapply);

 protected:  
  virtual EntryKernel* GetEntryByHandle(int64 handle);
  virtual EntryKernel* GetEntryByHandle(int64 metahandle,
      ScopedKernelLock* lock);
  virtual EntryKernel* GetEntryById(const Id& id);
  EntryKernel* GetEntryByServerTag(const std::string& tag);
  virtual EntryKernel* GetEntryByClientTag(const std::string& tag);
  EntryKernel* GetRootEntry();
  bool ReindexId(BaseWriteTransaction* trans, EntryKernel* const entry,
                 const Id& new_id);
  bool ReindexParentId(BaseWriteTransaction* trans, EntryKernel* const entry,
                       const Id& new_parent_id);
  void ClearDirtyMetahandles();

  DirOpenResult OpenImpl(
      const std::string& name,
      DirectoryChangeDelegate* delegate,
      const WeakHandle<TransactionObserver>& transaction_observer);

 private:
  struct Kernel {
    
    
    Kernel(const std::string& name, const KernelLoadInfo& info,
           DirectoryChangeDelegate* delegate,
           const WeakHandle<TransactionObserver>& transaction_observer);

    ~Kernel();

    
    base::Lock transaction_mutex;

    
    int64 next_write_transaction_id;

    
    std::string const name;

    
    
    
    // from the index, the mutex can be unlocked and entry read or written.
    
    
    
    base::Lock mutex;

    
    
    
    
    MetahandlesMap metahandles_map;

    
    IdsMap ids_map;

    
    
    TagsMap server_tags_map;

    
    
    
    TagsMap client_tags_map;

    
    
    ParentChildIndex parent_child_index;

    
    
    MetahandleSet unapplied_update_metahandles[MODEL_TYPE_COUNT];
    MetahandleSet unsynced_metahandles;
    
    
    MetahandleSet dirty_metahandles;

    
    
    MetahandleSet metahandles_to_purge;

    KernelShareInfoStatus info_status;

    
    

    // A structure containing the Directory state that is written back into the
    
    PersistedKernelInfo persisted_info;

    
    // unique server IDs. No need to lock, only written at init time.
    const std::string cache_guid;

    
    
    base::Lock save_changes_mutex;

    
    int64 next_metahandle;

    
    DirectoryChangeDelegate* const delegate;

    
    const WeakHandle<TransactionObserver> transaction_observer;
  };

  
  
  EntryKernel* GetEntryById(const Id& id, ScopedKernelLock* const lock);

  
  bool CheckTreeInvariants(syncable::BaseTransaction* trans,
                           const MetahandleSet& handles);

  
  
  
  
  void InitializeIndices(MetahandlesMap* handles_map);

  
  
  
  void TakeSnapshotForSaveChanges(SaveChangesSnapshot* snapshot);

  
  
  
  bool VacuumAfterSaveChanges(const SaveChangesSnapshot& snapshot);

  
  
  void HandleSaveChangesFailure(const SaveChangesSnapshot& snapshot);

  
  bool InsertEntry(BaseWriteTransaction* trans,
                   EntryKernel* entry, ScopedKernelLock* lock);
  bool InsertEntry(BaseWriteTransaction* trans, EntryKernel* entry);

  
  void GetAllMetaHandles(BaseTransaction* trans, MetahandleSet* result);
  bool SafeToPurgeFromMemory(WriteTransaction* trans,
                             const EntryKernel* const entry) const;

  
  void GetChildSetForKernel(
      BaseTransaction*,
      EntryKernel* kernel_,
      std::deque<const OrderedChildSet*>* child_sets) const;

  
  void AppendChildHandles(
      const ScopedKernelLock& lock,
      const Id& parent_id, Directory::Metahandles* result);

  
  void UnapplyEntry(EntryKernel* entry);
  void DeleteEntry(bool save_to_journal,
                   EntryKernel* entry,
                   EntryKernelSet* entries_to_journal);

  Kernel* kernel_;

  scoped_ptr<DirectoryBackingStore> store_;

  UnrecoverableErrorHandler* const unrecoverable_error_handler_;
  const ReportUnrecoverableErrorFunction report_unrecoverable_error_function_;
  bool unrecoverable_error_set_;

  
  NigoriHandler* const nigori_handler_;
  Cryptographer* const cryptographer_;

  InvariantCheckLevel invariant_check_level_;

  
  
  scoped_ptr<DeleteJournal> delete_journal_;

  DISALLOW_COPY_AND_ASSIGN(Directory);
};

}  
}  

#endif 
