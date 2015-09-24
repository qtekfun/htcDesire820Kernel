// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_TEST_ENGINE_TEST_SYNCABLE_UTILS_H_
#define SYNC_TEST_ENGINE_TEST_SYNCABLE_UTILS_H_

#include <string>

#include "sync/internal_api/public/base/model_type.h"

namespace syncer {
namespace syncable {

class BaseTransaction;
class Directory;
class Id;
class WriteTransaction;

int CountEntriesWithName(BaseTransaction* rtrans,
                         const syncable::Id& parent_id,
                         const std::string& name);

Id GetFirstEntryWithName(BaseTransaction* rtrans,
                         const syncable::Id& parent_id,
                         const std::string& name);

Id GetOnlyEntryWithName(BaseTransaction* rtrans,
                        const syncable::Id& parent_id,
                        const std::string& name);

void CreateTypeRoot(WriteTransaction* trans,
                    syncable::Directory *dir,
                    ModelType type);

}  
}  

#endif  
