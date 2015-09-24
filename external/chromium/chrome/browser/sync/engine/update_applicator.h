// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_UPDATE_APPLICATOR_H_
#define CHROME_BROWSER_SYNC_ENGINE_UPDATE_APPLICATOR_H_
#pragma once

#include <vector>

#include "base/basictypes.h"
#include "base/port.h"
#include "chrome/browser/sync/engine/model_safe_worker.h"
#include "chrome/browser/sync/syncable/syncable.h"

namespace browser_sync {

namespace sessions {
class ConflictProgress;
class UpdateProgress;
}

class ConflictResolver;
class Cryptographer;

class UpdateApplicator {
 public:
  typedef syncable::Directory::UnappliedUpdateMetaHandles::iterator
      UpdateIterator;

  UpdateApplicator(ConflictResolver* resolver,
                   Cryptographer* cryptographer,
                   const UpdateIterator& begin,
                   const UpdateIterator& end,
                   const ModelSafeRoutingInfo& routes,
                   ModelSafeGroup group_filter);
  ~UpdateApplicator();

  
  bool AttemptOneApplication(syncable::WriteTransaction* trans);
  
  bool AllUpdatesApplied() const;

  
  
  
  
  void SaveProgressIntoSessionState(
      sessions::ConflictProgress* conflict_progress,
      sessions::UpdateProgress* update_progress);

 private:
  
  bool SkipUpdate(const syncable::Entry& entry);

  
  void Advance();

  
  ConflictResolver* const resolver_;

  
  Cryptographer* cryptographer_;

  UpdateIterator const begin_;
  UpdateIterator end_;
  UpdateIterator pointer_;
  ModelSafeGroup group_filter_;
  bool progress_;

  const ModelSafeRoutingInfo routing_info_;

  
  std::vector<syncable::Id> conflicting_ids_;
  std::vector<syncable::Id> successful_ids_;

  DISALLOW_COPY_AND_ASSIGN(UpdateApplicator);
};

}  

#endif  
