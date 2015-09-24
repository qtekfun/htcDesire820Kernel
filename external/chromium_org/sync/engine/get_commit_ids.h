// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_GET_COMMIT_IDS_H_
#define SYNC_ENGINE_GET_COMMIT_IDS_H_

#include <vector>

#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/syncable/directory.h"

using std::vector;

namespace syncer {

namespace syncable {
class BaseTransaction;
}

SYNC_EXPORT_PRIVATE void GetCommitIdsForType(
    syncable::BaseTransaction* trans,
    ModelType type,
    size_t max_entries,
    std::vector<int64>* out);

}  

#endif  
