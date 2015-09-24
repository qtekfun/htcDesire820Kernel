// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_CONFLICT_RESOLVER_H_
#define SYNC_ENGINE_CONFLICT_RESOLVER_H_

#include <set>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "sync/engine/syncer_types.h"

namespace syncer {

namespace syncable {
class Id;
class WriteTransaction;
}  

class Cryptographer;

namespace sessions {
class StatusController;
}  

class ConflictResolver {
  friend class SyncerTest;
  FRIEND_TEST_ALL_PREFIXES(SyncerTest,
                           ConflictResolverMergeOverwritesLocalEntry);
 public:
  
  enum SimpleConflictResolutions {
    OVERWRITE_LOCAL,           
    OVERWRITE_SERVER,          
    UNDELETE,                  
    IGNORE_ENCRYPTION,         
                               
    NIGORI_MERGE,              
    CHANGES_MATCH,             
                               
    CONFLICT_RESOLUTION_SIZE,
  };

  ConflictResolver();
  ~ConflictResolver();
  
  
  void ResolveConflicts(syncable::WriteTransaction* trans,
                        const Cryptographer* cryptographer,
                        const std::set<syncable::Id>& simple_conflict_ids,
                        sessions::StatusController* status);

 private:
  void ProcessSimpleConflict(
      syncable::WriteTransaction* trans,
      const syncable::Id& id,
      const Cryptographer* cryptographer,
      sessions::StatusController* status);

  DISALLOW_COPY_AND_ASSIGN(ConflictResolver);
};

}  

#endif  
