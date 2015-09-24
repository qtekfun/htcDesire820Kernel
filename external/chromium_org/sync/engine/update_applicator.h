// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_UPDATE_APPLICATOR_H_
#define SYNC_ENGINE_UPDATE_APPLICATOR_H_

#include <vector>

#include "base/basictypes.h"
#include "base/port.h"
#include "sync/internal_api/public/engine/model_safe_worker.h"
#include "sync/syncable/syncable_id.h"
#include "sync/sessions/status_controller.h"

namespace syncer {

namespace sessions {
class StatusController;
}

namespace syncable {
class WriteTransaction;
class Entry;
}

class ConflictResolver;
class Cryptographer;

class UpdateApplicator {
 public:
  UpdateApplicator(Cryptographer* cryptographer);
  ~UpdateApplicator();

  
  void AttemptApplications(syncable::WriteTransaction* trans,
                           const std::vector<int64>& handles);

  int updates_applied() {
    return updates_applied_;
  }

  int encryption_conflicts() {
    return encryption_conflicts_;
  }

  int hierarchy_conflicts() {
    return hierarchy_conflicts_;
  }

  const std::set<syncable::Id>& simple_conflict_ids() {
    return simple_conflict_ids_;
  }

 private:
  
  bool SkipUpdate(const syncable::Entry& entry);

  
  Cryptographer* cryptographer_;

  DISALLOW_COPY_AND_ASSIGN(UpdateApplicator);

  int updates_applied_;
  int encryption_conflicts_;
  int hierarchy_conflicts_;
  std::set<syncable::Id> simple_conflict_ids_;
};

}  

#endif  
