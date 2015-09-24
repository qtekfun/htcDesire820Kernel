// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_SYNCER_H_
#define CHROME_BROWSER_SYNC_ENGINE_SYNCER_H_
#pragma once

#include <utility>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "chrome/browser/sync/engine/conflict_resolver.h"
#include "chrome/browser/sync/engine/syncer_types.h"
#include "chrome/browser/sync/engine/syncproto.h"
#include "chrome/browser/sync/sessions/sync_session.h"
#include "chrome/browser/sync/syncable/directory_event.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "chrome/browser/sync/util/extensions_activity_monitor.h"
#include "chrome/common/deprecated/event_sys.h"
#include "chrome/common/deprecated/event_sys-inl.h"

namespace syncable {
class Directory;
class DirectoryManager;
class Entry;
class Id;
class MutableEntry;
class WriteTransaction;
}  

namespace browser_sync {

class ModelSafeWorker;
class ServerConnectionManager;
class SyncProcessState;
class URLFactory;
struct HttpResponse;

enum SyncerStep {
  SYNCER_BEGIN,
  CLEANUP_DISABLED_TYPES,
  DOWNLOAD_UPDATES,
  PROCESS_CLIENT_COMMAND,
  VERIFY_UPDATES,
  PROCESS_UPDATES,
  STORE_TIMESTAMPS,
  APPLY_UPDATES,
  BUILD_COMMIT_REQUEST,
  POST_COMMIT_MESSAGE,
  PROCESS_COMMIT_RESPONSE,
  BUILD_AND_PROCESS_CONFLICT_SETS,
  RESOLVE_CONFLICTS,
  APPLY_UPDATES_TO_RESOLVE_CONFLICTS,
  CLEAR_PRIVATE_DATA,  
  SYNCER_END
};

class Syncer {
 public:
  typedef std::vector<int64> UnsyncedMetaHandles;

  
  
  Syncer();
  virtual ~Syncer();

  
  
  bool ExitRequested();
  void RequestEarlyExit();

  
  
  
  
  virtual void SyncShare(sessions::SyncSession* session);

  
  
  virtual void SyncShare(sessions::SyncSession* session,
                         SyncerStep first_step,
                         SyncerStep last_step);

 private:
  
  void ProcessClientCommand(sessions::SyncSession* session);

  bool early_exit_requested_;
  base::Lock early_exit_requested_lock_;

  ConflictResolver resolver_;

  
  
  Callback0::Type* pre_conflict_resolution_closure_;

  friend class SyncerTest;
  FRIEND_TEST_ALL_PREFIXES(SyncerTest, NameClashWithResolver);
  FRIEND_TEST_ALL_PREFIXES(SyncerTest, IllegalAndLegalUpdates);
  FRIEND_TEST_ALL_PREFIXES(SusanDeletingTest,
                           NewServerItemInAFolderHierarchyWeHaveDeleted3);
  FRIEND_TEST_ALL_PREFIXES(SyncerTest, TestCommitListOrderingAndNewParent);
  FRIEND_TEST_ALL_PREFIXES(SyncerTest,
                           TestCommitListOrderingAndNewParentAndChild);
  FRIEND_TEST_ALL_PREFIXES(SyncerTest, TestCommitListOrderingCounterexample);
  FRIEND_TEST_ALL_PREFIXES(SyncerTest, TestCommitListOrderingWithNesting);
  FRIEND_TEST_ALL_PREFIXES(SyncerTest, TestCommitListOrderingWithNewItems);
  FRIEND_TEST_ALL_PREFIXES(SyncerTest, TestGetUnsyncedAndSimpleCommit);
  FRIEND_TEST_ALL_PREFIXES(SyncerTest, TestPurgeWhileUnsynced);
  FRIEND_TEST_ALL_PREFIXES(SyncerTest, TestPurgeWhileUnapplied);
  FRIEND_TEST_ALL_PREFIXES(SyncerTest, UnappliedUpdateDuringCommit);
  FRIEND_TEST_ALL_PREFIXES(SyncerTest, DeletingEntryInFolder);
  FRIEND_TEST_ALL_PREFIXES(SyncerTest,
                           LongChangelistCreatesFakeOrphanedEntries);
  FRIEND_TEST_ALL_PREFIXES(SyncerTest, QuicklyMergeDualCreatedHierarchy);
  FRIEND_TEST_ALL_PREFIXES(SyncerTest, LongChangelistWithApplicationConflict);
  FRIEND_TEST_ALL_PREFIXES(SyncerTest, DeletingEntryWithLocalEdits);
  FRIEND_TEST_ALL_PREFIXES(EntryCreatedInNewFolderTest,
                           EntryCreatedInNewFolderMidSync);

  DISALLOW_COPY_AND_ASSIGN(Syncer);
};


template <class Iterator1, class Iterator2>
bool SortedCollectionsIntersect(Iterator1 begin1, Iterator1 end1,
                                Iterator2 begin2, Iterator2 end2) {
  Iterator1 i1 = begin1;
  Iterator2 i2 = begin2;
  while (i1 != end1 && i2 != end2) {
    if (*i1 == *i2)
      return true;
    if (*i1 > *i2)
      ++i2;
    else
      ++i1;
  }
  return false;
}
void CopyServerFields(syncable::Entry* src, syncable::MutableEntry* dest);
void ClearServerData(syncable::MutableEntry* entry);

}  

#endif  

