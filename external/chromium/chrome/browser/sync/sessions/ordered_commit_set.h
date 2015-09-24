// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_SESSIONS_ORDERED_COMMIT_SET_H_
#define CHROME_BROWSER_SYNC_SESSIONS_ORDERED_COMMIT_SET_H_
#pragma once

#include <map>
#include <set>
#include <vector>

#include "chrome/browser/sync/engine/model_safe_worker.h"
#include "chrome/browser/sync/syncable/model_type.h"
#include "chrome/browser/sync/syncable/syncable_id.h"

namespace browser_sync {
namespace sessions {

class OrderedCommitSet {
 public:
  
  
  
  
  
  typedef std::vector<size_t> Projection;

  
  explicit OrderedCommitSet(const browser_sync::ModelSafeRoutingInfo& routes);
  ~OrderedCommitSet();

  bool HaveCommitItem(const int64 metahandle) const {
    return inserted_metahandles_.count(metahandle) > 0;
  }

  void AddCommitItem(const int64 metahandle, const syncable::Id& commit_id,
                     syncable::ModelType type);

  const std::vector<syncable::Id>& GetAllCommitIds() const {
    return commit_ids_;
  }

  
  
  
  
  
  
  
  const syncable::Id& GetCommitIdAt(const size_t position) const {
    return commit_ids_[position];
  }

  
  syncable::ModelType GetModelTypeAt(const size_t position) const {
    return types_[position];
  }

  
  
  
  
  const Projection& GetCommitIdProjection(browser_sync::ModelSafeGroup group) {
    return projections_[group];
  }

  int Size() const {
    return commit_ids_.size();
  }

  
  
  bool HasBookmarkCommitId() const;

  void AppendReverse(const OrderedCommitSet& other);
  void Truncate(size_t max_size);

  void operator=(const OrderedCommitSet& other);
 private:
  
  typedef std::map<browser_sync::ModelSafeGroup, Projection> Projections;

  
  struct CommitItem {
    int64 meta;
    syncable::Id id;
    syncable::ModelType group;
  };

  CommitItem GetCommitItemAt(const int position) const;

  
  
  std::set<int64> inserted_metahandles_;
  std::vector<syncable::Id> commit_ids_;
  std::vector<int64> metahandle_order_;
  Projections projections_;

  
  
  
  
  
  
  std::vector<syncable::ModelType> types_;

  browser_sync::ModelSafeRoutingInfo routes_;
};

}  
}  

#endif  

