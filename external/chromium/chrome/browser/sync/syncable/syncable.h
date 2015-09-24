// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SYNCABLE_SYNCABLE_H_
#define CHROME_BROWSER_SYNC_SYNCABLE_SYNCABLE_H_
#pragma once

#include <algorithm>
#include <bitset>
#include <iosfwd>
#include <limits>
#include <set>
#include <string>
#include <vector>

#include "base/atomicops.h"
#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/synchronization/lock.h"
#include "base/time.h"
#include "chrome/browser/sync/protocol/sync.pb.h"
#include "chrome/browser/sync/syncable/autofill_migration.h"
#include "chrome/browser/sync/syncable/blob.h"
#include "chrome/browser/sync/syncable/dir_open_result.h"
#include "chrome/browser/sync/syncable/directory_event.h"
#include "chrome/browser/sync/syncable/syncable_id.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "chrome/browser/sync/util/dbgq.h"
#include "chrome/common/deprecated/event_sys.h"

class DictionaryValue;
struct PurgeInfo;

namespace sync_api {
class ReadTransaction;
class WriteNode;
class ReadNode;
}

namespace syncable {
class DirectoryChangeListener;
class Entry;

std::ostream& operator<<(std::ostream& s, const Entry& e);

class DirectoryBackingStore;

static const int64 kInvalidMetaHandle = 0;


enum {
  BEGIN_FIELDS = 0,
  INT64_FIELDS_BEGIN = BEGIN_FIELDS
};

enum MetahandleField {
  
  
  META_HANDLE = INT64_FIELDS_BEGIN
};

enum BaseVersion {
  
  
  BASE_VERSION = META_HANDLE + 1,
};

enum Int64Field {
  SERVER_VERSION = BASE_VERSION + 1,
  MTIME,
  SERVER_MTIME,
  CTIME,
  SERVER_CTIME,

  
  
  SERVER_POSITION_IN_PARENT,

  LOCAL_EXTERNAL_ID,  
                      

  INT64_FIELDS_END
};

enum {
  INT64_FIELDS_COUNT = INT64_FIELDS_END,
  ID_FIELDS_BEGIN = INT64_FIELDS_END,
};

enum IdField {
  
  ID = ID_FIELDS_BEGIN,
  PARENT_ID,
  SERVER_PARENT_ID,

  PREV_ID,
  NEXT_ID,
  ID_FIELDS_END
};

enum {
  ID_FIELDS_COUNT = ID_FIELDS_END - ID_FIELDS_BEGIN,
  BIT_FIELDS_BEGIN = ID_FIELDS_END
};

enum IndexedBitField {
  IS_UNSYNCED = BIT_FIELDS_BEGIN,
  IS_UNAPPLIED_UPDATE,
  INDEXED_BIT_FIELDS_END,
};

enum IsDelField {
  IS_DEL = INDEXED_BIT_FIELDS_END,
};

enum BitField {
  IS_DIR = IS_DEL + 1,
  SERVER_IS_DIR,
  SERVER_IS_DEL,
  BIT_FIELDS_END
};

enum {
  BIT_FIELDS_COUNT = BIT_FIELDS_END - BIT_FIELDS_BEGIN,
  STRING_FIELDS_BEGIN = BIT_FIELDS_END
};

enum StringField {
  
  NON_UNIQUE_NAME = STRING_FIELDS_BEGIN,
  
  SERVER_NON_UNIQUE_NAME,

  
  
  
  UNIQUE_SERVER_TAG,  
  UNIQUE_CLIENT_TAG,  
  STRING_FIELDS_END,
};

enum {
  STRING_FIELDS_COUNT = STRING_FIELDS_END - STRING_FIELDS_BEGIN,
  PROTO_FIELDS_BEGIN = STRING_FIELDS_END
};

enum ProtoField {
  SPECIFICS = PROTO_FIELDS_BEGIN,
  SERVER_SPECIFICS,
  PROTO_FIELDS_END,
};

enum {
  PROTO_FIELDS_COUNT = PROTO_FIELDS_END - PROTO_FIELDS_BEGIN
};

enum {
  FIELD_COUNT = PROTO_FIELDS_END,
  
  BEGIN_TEMPS = PROTO_FIELDS_END,
  BIT_TEMPS_BEGIN = BEGIN_TEMPS,
};

enum BitTemp {
  SYNCING = BIT_TEMPS_BEGIN,
  BIT_TEMPS_END,
};

enum {
  BIT_TEMPS_COUNT = BIT_TEMPS_END - BIT_TEMPS_BEGIN
};

class BaseTransaction;
class WriteTransaction;
class ReadTransaction;
class Directory;
class ScopedDirLookup;

enum GetById {
  GET_BY_ID
};

enum GetByClientTag {
  GET_BY_CLIENT_TAG
};

enum GetByServerTag {
  GET_BY_SERVER_TAG
};

enum GetByHandle {
  GET_BY_HANDLE
};

enum Create {
  CREATE
};

enum CreateNewUpdateItem {
  CREATE_NEW_UPDATE_ITEM
};

typedef std::set<int64> MetahandleSet;


struct EntryKernel {
 private:
  std::string string_fields[STRING_FIELDS_COUNT];
  sync_pb::EntitySpecifics specifics_fields[PROTO_FIELDS_COUNT];
  int64 int64_fields[INT64_FIELDS_COUNT];
  Id id_fields[ID_FIELDS_COUNT];
  std::bitset<BIT_FIELDS_COUNT> bit_fields;
  std::bitset<BIT_TEMPS_COUNT> bit_temps;

 public:
  EntryKernel();
  ~EntryKernel();

  
  
  
  inline void mark_dirty(syncable::MetahandleSet* dirty_index) {
    if (!dirty_ && dirty_index) {
      DCHECK_NE(0, ref(META_HANDLE));
      dirty_index->insert(ref(META_HANDLE));
    }
    dirty_ = true;
  }

  
  
  
  inline void clear_dirty(syncable::MetahandleSet* dirty_index) {
    if (dirty_ && dirty_index) {
      DCHECK_NE(0, ref(META_HANDLE));
      dirty_index->erase(ref(META_HANDLE));
    }
    dirty_ = false;
  }

  inline bool is_dirty() const {
    return dirty_;
  }

  
  inline void put(MetahandleField field, int64 value) {
    int64_fields[field - INT64_FIELDS_BEGIN] = value;
  }
  inline void put(Int64Field field, int64 value) {
    int64_fields[field - INT64_FIELDS_BEGIN] = value;
  }
  inline void put(IdField field, const Id& value) {
    id_fields[field - ID_FIELDS_BEGIN] = value;
  }
  inline void put(BaseVersion field, int64 value) {
    int64_fields[field - INT64_FIELDS_BEGIN] = value;
  }
  inline void put(IndexedBitField field, bool value) {
    bit_fields[field - BIT_FIELDS_BEGIN] = value;
  }
  inline void put(IsDelField field, bool value) {
    bit_fields[field - BIT_FIELDS_BEGIN] = value;
  }
  inline void put(BitField field, bool value) {
    bit_fields[field - BIT_FIELDS_BEGIN] = value;
  }
  inline void put(StringField field, const std::string& value) {
    string_fields[field - STRING_FIELDS_BEGIN] = value;
  }
  inline void put(ProtoField field, const sync_pb::EntitySpecifics& value) {
    specifics_fields[field - PROTO_FIELDS_BEGIN].CopyFrom(value);
  }
  inline void put(BitTemp field, bool value) {
    bit_temps[field - BIT_TEMPS_BEGIN] = value;
  }

  
  inline int64 ref(MetahandleField field) const {
    return int64_fields[field - INT64_FIELDS_BEGIN];
  }
  inline int64 ref(Int64Field field) const {
    return int64_fields[field - INT64_FIELDS_BEGIN];
  }
  inline const Id& ref(IdField field) const {
    return id_fields[field - ID_FIELDS_BEGIN];
  }
  inline int64 ref(BaseVersion field) const {
    return int64_fields[field - INT64_FIELDS_BEGIN];
  }
  inline bool ref(IndexedBitField field) const {
    return bit_fields[field - BIT_FIELDS_BEGIN];
  }
  inline bool ref(IsDelField field) const {
    return bit_fields[field - BIT_FIELDS_BEGIN];
  }
  inline bool ref(BitField field) const {
    return bit_fields[field - BIT_FIELDS_BEGIN];
  }
  inline const std::string& ref(StringField field) const {
    return string_fields[field - STRING_FIELDS_BEGIN];
  }
  inline const sync_pb::EntitySpecifics& ref(ProtoField field) const {
    return specifics_fields[field - PROTO_FIELDS_BEGIN];
  }
  inline bool ref(BitTemp field) const {
    return bit_temps[field - BIT_TEMPS_BEGIN];
  }

  
  inline std::string& mutable_ref(StringField field) {
    return string_fields[field - STRING_FIELDS_BEGIN];
  }
  inline sync_pb::EntitySpecifics& mutable_ref(ProtoField field) {
    return specifics_fields[field - PROTO_FIELDS_BEGIN];
  }
  inline Id& mutable_ref(IdField field) {
    return id_fields[field - ID_FIELDS_BEGIN];
  }

  
  
  DictionaryValue* ToValue() const;

 private:
  
  bool dirty_;
};

class Entry {
  friend class Directory;
  friend std::ostream& operator << (std::ostream& s, const Entry& e);

 public:
  
  
  Entry(BaseTransaction* trans, GetByHandle, int64 handle);
  Entry(BaseTransaction* trans, GetById, const Id& id);
  Entry(BaseTransaction* trans, GetByServerTag, const std::string& tag);
  Entry(BaseTransaction* trans, GetByClientTag, const std::string& tag);

  bool good() const { return 0 != kernel_; }

  BaseTransaction* trans() const { return basetrans_; }

  
  inline int64 Get(MetahandleField field) const {
    DCHECK(kernel_);
    return kernel_->ref(field);
  }
  inline Id Get(IdField field) const {
    DCHECK(kernel_);
    return kernel_->ref(field);
  }
  inline int64 Get(Int64Field field) const {
    DCHECK(kernel_);
    return kernel_->ref(field);
  }
  inline int64 Get(BaseVersion field) const {
    DCHECK(kernel_);
    return kernel_->ref(field);
  }
  inline bool Get(IndexedBitField field) const {
    DCHECK(kernel_);
    return kernel_->ref(field);
  }
  inline bool Get(IsDelField field) const {
    DCHECK(kernel_);
    return kernel_->ref(field);
  }
  inline bool Get(BitField field) const {
    DCHECK(kernel_);
    return kernel_->ref(field);
  }
  const std::string& Get(StringField field) const;
  inline const sync_pb::EntitySpecifics& Get(ProtoField field) const {
    DCHECK(kernel_);
    return kernel_->ref(field);
  }
  inline bool Get(BitTemp field) const {
    DCHECK(kernel_);
    return kernel_->ref(field);
  }

  ModelType GetServerModelType() const;
  ModelType GetModelType() const;

  
  
  bool ShouldMaintainPosition() const {
    return GetModelType() == BOOKMARKS;
  }

  inline bool ExistsOnClientBecauseNameIsNonEmpty() const {
    DCHECK(kernel_);
    return !kernel_->ref(NON_UNIQUE_NAME).empty();
  }

  inline bool IsRoot() const {
    DCHECK(kernel_);
    return kernel_->ref(ID).IsRoot();
  }

  Directory* dir() const;

  const EntryKernel GetKernelCopy() const {
    return *kernel_;
  }

  
  
  
  
  Id ComputePrevIdFromServerPosition(const Id& parent_id) const;

  
  
  DictionaryValue* ToValue() const;

 protected:  
  friend class sync_api::ReadNode;
  void* operator new(size_t size) { return (::operator new)(size); }

  inline Entry(BaseTransaction* trans)
      : basetrans_(trans),
        kernel_(NULL) { }

 protected:

  BaseTransaction* const basetrans_;

  EntryKernel* kernel_;

 private:
  
  ModelType GetServerModelTypeHelper() const;

  DISALLOW_COPY_AND_ASSIGN(Entry);
};

class MutableEntry : public Entry {
  friend class WriteTransaction;
  friend class Directory;
  void Init(WriteTransaction* trans, const Id& parent_id,
      const std::string& name);
 public:
  MutableEntry(WriteTransaction* trans, Create, const Id& parent_id,
               const std::string& name);
  MutableEntry(WriteTransaction* trans, CreateNewUpdateItem, const Id& id);
  MutableEntry(WriteTransaction* trans, GetByHandle, int64);
  MutableEntry(WriteTransaction* trans, GetById, const Id&);
  MutableEntry(WriteTransaction* trans, GetByClientTag, const std::string& tag);
  MutableEntry(WriteTransaction* trans, GetByServerTag, const std::string& tag);

  inline WriteTransaction* write_transaction() const {
    return write_transaction_;
  }

  
  
  
  
  bool Put(Int64Field field, const int64& value);
  bool Put(IdField field, const Id& value);

  
  
  
  
  
  
  
  
  
  void PutParentIdPropertyOnly(const Id& parent_id);

  bool Put(StringField field, const std::string& value);
  bool Put(BaseVersion field, int64 value);

  bool Put(ProtoField field, const sync_pb::EntitySpecifics& value);
  bool Put(BitField field, bool value);
  inline bool Put(IsDelField field, bool value) {
    return PutIsDel(value);
  }
  bool Put(IndexedBitField field, bool value);

  
  
  
  
  bool PutPredecessor(const Id& predecessor_id);

  bool Put(BitTemp field, bool value);

 protected:
  syncable::MetahandleSet* GetDirtyIndexHelper();

  bool PutIsDel(bool value);

 private:  
  friend class sync_api::WriteNode;
  void* operator new(size_t size) { return (::operator new)(size); }

  bool PutImpl(StringField field, const std::string& value);
  bool PutUniqueClientTag(const std::string& value);

  
  
  void UnlinkFromOrder();

  
  
  
  WriteTransaction* const write_transaction_;

 protected:
  MutableEntry();

  DISALLOW_COPY_AND_ASSIGN(MutableEntry);
};

class LessParentIdAndHandle;
template <typename FieldType, FieldType field_index>
class LessField;
class LessEntryMetaHandles {
 public:
  inline bool operator()(const syncable::EntryKernel& a,
                         const syncable::EntryKernel& b) const {
    return a.ref(META_HANDLE) < b.ref(META_HANDLE);
  }
};
typedef std::set<EntryKernel, LessEntryMetaHandles> OriginalEntries;


struct MetahandleIndexer {
  
  typedef LessField<MetahandleField, META_HANDLE> Comparator;

  
  inline static bool ShouldInclude(const EntryKernel* a) {
    return true;
  }
};

struct IdIndexer {
  
  typedef LessField<IdField, ID> Comparator;

  
  inline static bool ShouldInclude(const EntryKernel* a) {
    return true;
  }
};

struct ClientTagIndexer {
  
  typedef LessField<StringField, UNIQUE_CLIENT_TAG> Comparator;

  
  static bool ShouldInclude(const EntryKernel* a);
};

struct ParentIdAndHandleIndexer {
  
  
  class Comparator {
   public:
    bool operator() (const syncable::EntryKernel* a,
                     const syncable::EntryKernel* b) const;
  };

  
  static bool ShouldInclude(const EntryKernel* a);
};

template <typename Indexer>
struct Index {
  typedef std::set<EntryKernel*, typename Indexer::Comparator> Set;
};

enum WriterTag {
  INVALID,
  SYNCER,
  AUTHWATCHER,
  UNITTEST,
  VACUUM_AFTER_SAVE,
  PURGE_ENTRIES,
  SYNCAPI
};

class ScopedKernelLock;
class IdFilter;
class DirectoryManager;

class Directory {
  friend class BaseTransaction;
  friend class Entry;
  friend class MutableEntry;
  friend class ReadTransaction;
  friend class ReadTransactionWithoutDB;
  friend class ScopedKernelLock;
  friend class ScopedKernelUnlock;
  friend class WriteTransaction;
  friend class SyncableDirectoryTest;
  FRIEND_TEST_ALL_PREFIXES(SyncableDirectoryTest,
                           TakeSnapshotGetsAllDirtyHandlesTest);
  FRIEND_TEST_ALL_PREFIXES(SyncableDirectoryTest,
                           TakeSnapshotGetsOnlyDirtyHandlesTest);
  FRIEND_TEST_ALL_PREFIXES(SyncableDirectoryTest, TestPurgeEntriesWithTypeIn);
  FRIEND_TEST_ALL_PREFIXES(SyncableDirectoryTest,
                           TakeSnapshotGetsMetahandlesToPurge);

 public:
  class EventListenerHookup;

  
  
  struct PersistedKernelInfo {
    AutofillMigrationDebugInfo autofill_migration_debug_info;

    PersistedKernelInfo();
    ~PersistedKernelInfo();

    
    
    
    void reset_download_progress(ModelType model_type);

    
    sync_pb::DataTypeProgressMarker download_progress[MODEL_TYPE_COUNT];
    
    ModelTypeBitSet initial_sync_ended;
    
    
    std::string store_birthday;
    
    int64 next_id;
    
    std::string notification_state;

    AutofillMigrationState autofill_migration_state;
  };

  
  
  struct KernelLoadInfo {
    PersistedKernelInfo kernel_info;
    std::string cache_guid;  
    int64 max_metahandle;    
    KernelLoadInfo() : max_metahandle(0) {
    }
  };

  
  
  enum KernelShareInfoStatus {
    KERNEL_SHARE_INFO_INVALID,
    KERNEL_SHARE_INFO_VALID,
    KERNEL_SHARE_INFO_DIRTY
  };

  
  
  
  struct SaveChangesSnapshot {
    SaveChangesSnapshot();
    ~SaveChangesSnapshot();

    KernelShareInfoStatus kernel_info_status;
    PersistedKernelInfo kernel_info;
    OriginalEntries dirty_metas;
    MetahandleSet metahandles_to_purge;
  };

  Directory();
  virtual ~Directory();

  DirOpenResult Open(const FilePath& file_path, const std::string& name);

  void Close();

  int64 NextMetahandle();
  
  
  Id NextId();

  const FilePath& file_path() const { return kernel_->db_path; }
  bool good() const { return NULL != store_; }

  
  
  void GetDownloadProgress(
      ModelType type,
      sync_pb::DataTypeProgressMarker* value_out) const;
  void GetDownloadProgressAsString(
      ModelType type,
      std::string* value_out) const;
  void SetDownloadProgress(
      ModelType type,
      const sync_pb::DataTypeProgressMarker& value);

  bool initial_sync_ended_for_type(ModelType type) const;
  void set_initial_sync_ended_for_type(ModelType type, bool value);
  AutofillMigrationState get_autofill_migration_state() const;

  AutofillMigrationDebugInfo get_autofill_migration_debug_info() const;

  void set_autofill_migration_state(AutofillMigrationState state);

  void set_autofill_migration_state_debug_info(
      syncable::AutofillMigrationDebugInfo::PropertyToSet property_to_set,
      const syncable::AutofillMigrationDebugInfo& info);

  const std::string& name() const { return kernel_->name; }

  
  
  
  std::string store_birthday() const;
  void set_store_birthday(const std::string& store_birthday);

  std::string GetAndClearNotificationState();
  void SetNotificationState(const std::string& notification_state);

  
  std::string cache_guid() const;

  void SetChangeListener(DirectoryChangeListener* listener);

 protected:  
  virtual EntryKernel* GetEntryByHandle(int64 handle);
  virtual EntryKernel* GetEntryByHandle(int64 metahandle,
      ScopedKernelLock* lock);
  virtual EntryKernel* GetEntryById(const Id& id);
  EntryKernel* GetEntryByServerTag(const std::string& tag);
  virtual EntryKernel* GetEntryByClientTag(const std::string& tag);
  EntryKernel* GetRootEntry();
  bool ReindexId(EntryKernel* const entry, const Id& new_id);
  void ReindexParentId(EntryKernel* const entry, const Id& new_parent_id);
  void ClearDirtyMetahandles();

  
  
  void UnlinkEntryFromOrder(EntryKernel* entry,
                            WriteTransaction* trans,
                            ScopedKernelLock* lock);

  
  virtual DirectoryBackingStore* CreateBackingStore(
      const std::string& dir_name,
      const FilePath& backing_filepath);

 private:
  
  
  EntryKernel* GetEntryById(const Id& id, ScopedKernelLock* const lock);

  DirOpenResult OpenImpl(const FilePath& file_path, const std::string& name);

  template <class T> void TestAndSet(T* kernel_data, const T* data_to_set);

  struct DirectoryEventTraits {
    typedef DirectoryEvent EventType;
    static inline bool IsChannelShutdownEvent(const DirectoryEvent& event) {
      return DIRECTORY_DESTROYED == event;
    }
  };
 public:
  typedef EventChannel<DirectoryEventTraits, base::Lock> Channel;
  typedef std::vector<int64> ChildHandles;

  
  void GetChildHandles(BaseTransaction*, const Id& parent_id,
      ChildHandles* result);

  
  
  virtual Id GetFirstChildId(BaseTransaction* trans, const Id& parent_id);
  Id GetLastChildId(BaseTransaction* trans, const Id& parent_id);

  
  
  
  
  Id ComputePrevIdFromServerPosition(
      const EntryKernel* update_item,
      const syncable::Id& parent_id);

  
  
  
  
  
  
  
  
  bool SaveChanges();

  
  int64 unsynced_entity_count() const;

  
  
  
  typedef std::vector<int64> UnsyncedMetaHandles;
  void GetUnsyncedMetaHandles(BaseTransaction* trans,
                              UnsyncedMetaHandles* result);

  
  typedef std::vector<int64> UnappliedUpdateMetaHandles;
  void GetUnappliedUpdateMetaHandles(BaseTransaction* trans,
                                     UnappliedUpdateMetaHandles* result);

  
  inline Channel* channel() const {
    return kernel_->channel;
  }

  
  
  
  
  
  
  void CheckTreeInvariants(syncable::BaseTransaction* trans,
                           bool full_scan);

  void CheckTreeInvariants(syncable::BaseTransaction* trans,
                           const OriginalEntries* originals);

  void CheckTreeInvariants(syncable::BaseTransaction* trans,
                           const MetahandleSet& handles,
                           const IdFilter& idfilter);

  
  
  
  
  
  
  
  virtual void PurgeEntriesWithTypeIn(const std::set<ModelType>& types);

 private:
  
  
  void InitializeIndices();

  
  
  
  void TakeSnapshotForSaveChanges(SaveChangesSnapshot* snapshot);

  
  
  
  void VacuumAfterSaveChanges(const SaveChangesSnapshot& snapshot);

  
  
  void HandleSaveChangesFailure(const SaveChangesSnapshot& snapshot);

  
  void InsertEntry(EntryKernel* entry, ScopedKernelLock* lock);
  void InsertEntry(EntryKernel* entry);

  
  void GetAllMetaHandles(BaseTransaction* trans, MetahandleSet* result);
  bool SafeToPurgeFromMemory(const EntryKernel* const entry) const;

  
  
  
  void set_initial_sync_ended_for_type_unsafe(ModelType type, bool x);
  void SetNotificationStateUnsafe(const std::string& notification_state);

  Directory& operator = (const Directory&);

 public:
  typedef Index<MetahandleIndexer>::Set MetahandlesIndex;
  typedef Index<IdIndexer>::Set IdsIndex;
  
  
  
  typedef Index<ParentIdAndHandleIndexer>::Set ParentIdChildIndex;

  
  
  
  
  typedef Index<ClientTagIndexer>::Set ClientTagIndex;

 protected:
  
  void init_kernel(const std::string& name);

 private:

  struct Kernel {
    Kernel(const FilePath& db_path, const std::string& name,
           const KernelLoadInfo& info);

    ~Kernel();

    void AddRef();  
    void Release();

    FilePath const db_path;
    
    volatile base::subtle::AtomicWord refcount;

    
    base::Lock transaction_mutex;

    
    std::string const name;

    
    
    
    // from the index, the mutex can be unlocked and entry read or written.
    
    
    
    base::Lock mutex;
    
    MetahandlesIndex* metahandles_index;
    
    IdsIndex* ids_index;
    ParentIdChildIndex* parent_id_child_index;
    ClientTagIndex* client_tag_index;
    
    
    EntryKernel needle;

    
    MetahandleSet* const unapplied_update_metahandles;
    MetahandleSet* const unsynced_metahandles;
    
    
    MetahandleSet* const dirty_metahandles;

    
    
    MetahandleSet* const metahandles_to_purge;

    
    Channel* const channel;

    
    
    DirectoryChangeListener* change_listener_;

    KernelShareInfoStatus info_status;

    
    

    // A structure containing the Directory state that is written back into the
    
    PersistedKernelInfo persisted_info;

    
    // unique server IDs. No need to lock, only written at init time.
    std::string cache_guid;

    
    
    base::Lock save_changes_mutex;

    
    int64 next_metahandle;

    
    
    DebugQueue<int64, 1000> flushed_metahandles;
  };

  
  ParentIdChildIndex::iterator LocateInParentChildIndex(
      const ScopedKernelLock& lock,
      const Id& parent_id,
      int64 position_in_parent,
      const Id& item_id_for_tiebreaking);

  
  
  ParentIdChildIndex::iterator GetParentChildIndexLowerBound(
      const ScopedKernelLock& lock,
      const Id& parent_id);

  
  
  ParentIdChildIndex::iterator GetParentChildIndexUpperBound(
      const ScopedKernelLock& lock,
      const Id& parent_id);

  Kernel* kernel_;

  DirectoryBackingStore* store_;
};

class ScopedKernelLock {
 public:
  explicit ScopedKernelLock(const Directory*);
  ~ScopedKernelLock() {}

  base::AutoLock scoped_lock_;
  Directory* const dir_;
  DISALLOW_COPY_AND_ASSIGN(ScopedKernelLock);
};

class BaseTransaction {
  friend class Entry;
 public:
  inline Directory* directory() const { return directory_; }
  inline Id root_id() const { return Id(); }

  virtual ~BaseTransaction();

 protected:
  BaseTransaction(Directory* directory, const char* name,
                  const char* source_file, int line, WriterTag writer);

  
  explicit BaseTransaction(Directory* directory);

  void UnlockAndLog(OriginalEntries* entries);
  virtual bool NotifyTransactionChangingAndEnding(
      OriginalEntries* entries,
      ModelTypeBitSet* models_with_changes);
  virtual void NotifyTransactionComplete(ModelTypeBitSet models_with_changes);

  Directory* const directory_;
  Directory::Kernel* const dirkernel_;  
  const char* const name_;
  base::TimeTicks time_acquired_;
  const char* const source_file_;
  const int line_;
  WriterTag writer_;

 private:
  void Lock();

  DISALLOW_COPY_AND_ASSIGN(BaseTransaction);
};

class ReadTransaction : public BaseTransaction {
 public:
  ReadTransaction(Directory* directory, const char* source_file,
                  int line);
  ReadTransaction(const ScopedDirLookup& scoped_dir,
                  const char* source_file, int line);

  virtual ~ReadTransaction();

 protected:  
  friend class sync_api::ReadTransaction;
  void* operator new(size_t size) { return (::operator new)(size); }

  DISALLOW_COPY_AND_ASSIGN(ReadTransaction);
};

class WriteTransaction : public BaseTransaction {
  friend class MutableEntry;
 public:
  explicit WriteTransaction(Directory* directory, WriterTag writer,
                            const char* source_file, int line);
  explicit WriteTransaction(const ScopedDirLookup& directory,
                            WriterTag writer, const char* source_file,
                            int line);
  virtual ~WriteTransaction();

  void SaveOriginal(EntryKernel* entry);

 protected:
  
  
  
  OriginalEntries* const originals_;

  explicit WriteTransaction(Directory *directory);

  DISALLOW_COPY_AND_ASSIGN(WriteTransaction);
};

bool IsLegalNewParent(BaseTransaction* trans, const Id& id, const Id& parentid);

int64 Now();

void MarkForSyncing(syncable::MutableEntry* e);

void ZeroFields(EntryKernel* entry, int first_field);

}  

std::ostream& operator <<(std::ostream&, const syncable::Blob&);

#endif  
