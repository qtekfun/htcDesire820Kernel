// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_SYNC_DIRECTORY_COMMIT_CONTRIBUTOR_H_
#define SYNC_ENGINE_SYNC_DIRECTORY_COMMIT_CONTRIBUTOR_H_

#include <map>

#include "sync/internal_api/public/base/model_type.h"

namespace syncer {

class SyncDirectoryCommitContribution;

namespace syncable {
class Directory;
}

class SyncDirectoryCommitContributor {
 public:
  SyncDirectoryCommitContributor(syncable::Directory* dir, ModelType type);
  ~SyncDirectoryCommitContributor();

  SyncDirectoryCommitContribution* GetContribution(size_t max_entries);

 private:
  syncable::Directory* dir_;
  ModelType type_;
};

typedef std::map<ModelType, SyncDirectoryCommitContributor*>
    CommitContributorMap;

}  

#endif  
