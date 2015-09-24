// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_SYNCER_UTIL_H_
#define CHROME_BROWSER_SYNC_ENGINE_SYNCER_UTIL_H_
#pragma once

#include <set>
#include <string>
#include <vector>

#include "build/build_config.h"
#include "chrome/browser/sync/engine/syncer.h"
#include "chrome/browser/sync/engine/syncer_types.h"
#include "chrome/browser/sync/syncable/syncable.h"
#include "chrome/browser/sync/syncable/syncable_id.h"

namespace browser_sync {

class Cryptographer;
class SyncEntity;

class SyncerUtil {
 public:
  static void ChangeEntryIDAndUpdateChildren(
      syncable::WriteTransaction* trans,
      syncable::MutableEntry* entry,
      const syncable::Id& new_id,
      syncable::Directory::ChildHandles* children);

  
  static int GetUnsyncedEntries(syncable::BaseTransaction* trans,
                                std::vector<int64> *handles);

  static void ChangeEntryIDAndUpdateChildren(syncable::WriteTransaction* trans,
                                             syncable::MutableEntry* entry,
                                             const syncable::Id& new_id);

  
  
  
  
  
  
  
  
  
  static syncable::Id FindLocalIdToUpdate(
      syncable::BaseTransaction* trans,
      const SyncEntity& server_entry);

  static UpdateAttemptResponse AttemptToUpdateEntry(
      syncable::WriteTransaction* const trans,
      syncable::MutableEntry* const entry,
      ConflictResolver* resolver,
      Cryptographer* cryptographer);

  
  static void UpdateServerFieldsFromUpdate(
      syncable::MutableEntry* local_entry,
      const SyncEntity& server_entry,
      const std::string& name);

  
  static void CreateNewEntry(syncable::WriteTransaction *trans,
                             const syncable::Id& id);

  static bool ServerAndLocalEntriesMatch(syncable::Entry* entry);

  static void SplitServerInformationIntoNewEntry(
      syncable::WriteTransaction* trans,
      syncable::MutableEntry* entry);

  
  
  static void UpdateLocalDataFromServerData(syncable::WriteTransaction* trans,
                                            syncable::MutableEntry* entry);

  static VerifyCommitResult ValidateCommitEntry(syncable::Entry* entry);

  static VerifyResult VerifyNewEntry(const SyncEntity& update,
                                     syncable::Entry* target,
                                     const bool deleted);

  
  
  static VerifyResult VerifyUpdateConsistency(syncable::WriteTransaction* trans,
                                              const SyncEntity& update,
                                              syncable::MutableEntry* target,
                                              const bool deleted,
                                              const bool is_directory,
                                              syncable::ModelType model_type);

  
  
  static VerifyResult VerifyUndelete(syncable::WriteTransaction* trans,
                                     const SyncEntity& update,
                                     syncable::MutableEntry* target);

  
  
  
  
  
  
  
  
  
  static bool AddItemThenPredecessors(
      syncable::BaseTransaction* trans,
      syncable::Entry* item,
      syncable::IndexedBitField inclusion_filter,
      syncable::MetahandleSet* inserted_items,
      std::vector<syncable::Id>* commit_ids);

  
  
  
  static void AddPredecessorsThenItem(
      syncable::BaseTransaction* trans,
      syncable::Entry* item,
      syncable::IndexedBitField inclusion_filter,
      syncable::MetahandleSet* inserted_items,
      std::vector<syncable::Id>* commit_ids);

  static void MarkDeletedChildrenSynced(
      const syncable::ScopedDirLookup &dir,
      std::set<syncable::Id>* deleted_folders);

  
  
  
  static bool ServerAndLocalOrdersMatch(syncable::Entry* entry);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(SyncerUtil);
};

#ifndef OS_WIN

inline int64 ServerTimeToClientTime(int64 server_time) {
  return server_time / GG_LONGLONG(1000);
}

inline int64 ClientTimeToServerTime(int64 client_time) {
  return client_time * GG_LONGLONG(1000);
}

inline bool ClientAndServerTimeMatch(int64 client_time, int64 server_time) {
  
  return client_time == ServerTimeToClientTime(server_time);
}
#else
inline int64 ServerTimeToClientTime(int64 server_time) {
  return server_time * GG_LONGLONG(10000) + GG_LONGLONG(116444736000000000);
}

inline int64 ClientTimeToServerTime(int64 client_time) {
  return (client_time - GG_LONGLONG(116444736000000000)) / GG_LONGLONG(10000);
}

inline bool ClientAndServerTimeMatch(int64 client_time, int64 server_time) {
  
  return ClientTimeToServerTime(client_time) == server_time;
}
#endif

}  

#endif  
