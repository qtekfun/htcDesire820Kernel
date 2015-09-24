// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SYNCABLE_SYNCABLE_DELETE_JOURNAL_H_
#define SYNC_SYNCABLE_SYNCABLE_DELETE_JOURNAL_H_

#include <set>

#include "base/gtest_prod_util.h"
#include "base/synchronization/lock.h"
#include "sync/base/sync_export.h"
#include "sync/syncable/metahandle_set.h"
#include "sync/syncable/syncable-inl.h"

namespace syncer {
namespace syncable {

class BaseTransaction;
struct EntryKernel;

typedef std::set<const EntryKernel*, LessField<IdField, ID> > JournalIndex;

class SYNC_EXPORT_PRIVATE DeleteJournal {
 public:
  FRIEND_TEST_ALL_PREFIXES(SyncableDirectoryTest, ManageDeleteJournals);

  
  
  explicit DeleteJournal(JournalIndex* initial_journal);
  ~DeleteJournal();

  
  size_t GetDeleteJournalSize(BaseTransaction* trans) const;

  
  
  void UpdateDeleteJournalForServerDelete(BaseTransaction* trans,
                                          bool was_deleted,
                                          const EntryKernel& entry);

  
  
  
  
  
  
  
  
  void GetDeleteJournals(BaseTransaction* trans, ModelType type,
                         EntryKernelSet* deleted_entries);

  
  
  
  
  void PurgeDeleteJournals(BaseTransaction* trans,
                           const MetahandleSet& to_purge);

  
  
  
  void TakeSnapshotAndClear(BaseTransaction* trans,
                            EntryKernelSet* journal_entries,
                            MetahandleSet* journals_to_purge);

  
  
  
  
  
  
  void AddJournalBatch(BaseTransaction* trans, const EntryKernelSet& entries);

  
  static bool IsDeleteJournalEnabled(ModelType type);

 private:
  
  
  
  
  JournalIndex delete_journals_;

  
  
  MetahandleSet delete_journals_to_purge_;

  
  
  ModelTypeSet passive_delete_journal_types_;

  DISALLOW_COPY_AND_ASSIGN(DeleteJournal);
};

}  
}  

#endif  
