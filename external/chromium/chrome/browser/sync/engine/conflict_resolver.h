// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_CONFLICT_RESOLVER_H_
#define CHROME_BROWSER_SYNC_ENGINE_CONFLICT_RESOLVER_H_
#pragma once

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "chrome/browser/sync/engine/syncer_types.h"
#include "chrome/common/deprecated/event_sys.h"

namespace syncable {
class BaseTransaction;
class Id;
class MutableEntry;
class ScopedDirLookup;
class WriteTransaction;
}  

namespace browser_sync {
namespace sessions {
class StatusController;
}

class ConflictResolver {
  friend class SyncerTest;
  FRIEND_TEST_ALL_PREFIXES(SyncerTest,
                           ConflictResolverMergeOverwritesLocalEntry);
 public:
  ConflictResolver();
  ~ConflictResolver();
  
  
  bool ResolveConflicts(const syncable::ScopedDirLookup& dir,
                        sessions::StatusController* status);

 private:
  
  
  
  
  typedef std::string ConflictSetCountMapKey;
  typedef std::map<ConflictSetCountMapKey, int> ConflictSetCountMap;
  typedef std::map<syncable::Id, int> SimpleConflictCountMap;

  enum ProcessSimpleConflictResult {
    NO_SYNC_PROGRESS,  
    SYNC_PROGRESS,     
  };

  
  
  ConflictSetCountMapKey GetSetKey(ConflictSet* conflict_set);

  void IgnoreLocalChanges(syncable::MutableEntry* entry);
  void OverwriteServerChanges(syncable::WriteTransaction* trans,
                              syncable::MutableEntry* entry);

  ProcessSimpleConflictResult ProcessSimpleConflict(
      syncable::WriteTransaction* trans,
      const syncable::Id& id);

  bool ResolveSimpleConflicts(const syncable::ScopedDirLookup& dir,
                              sessions::StatusController* status);

  bool ProcessConflictSet(syncable::WriteTransaction* trans,
                          ConflictSet* conflict_set,
                          int conflict_count);

  
  template <typename InputIt>
  bool LogAndSignalIfConflictStuck(syncable::BaseTransaction* trans,
                                   int attempt_count,
                                   InputIt start, InputIt end,
                                   sessions::StatusController* status);

  ConflictSetCountMap conflict_set_count_map_;
  SimpleConflictCountMap simple_conflict_count_map_;

  
  
  
  
  std::set<syncable::Id> children_of_merged_dirs_;

  DISALLOW_COPY_AND_ASSIGN(ConflictResolver);
};

}  

#endif  
