// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_ENGINE_CONFLICT_UTIL_H_
#define SYNC_ENGINE_CONFLICT_UTIL_H_

namespace syncer {

namespace syncable {
class MutableEntry;
}

namespace conflict_util {

void IgnoreLocalChanges(syncable::MutableEntry* entry);

void OverwriteServerChanges(syncable::MutableEntry* entry);

void IgnoreConflict(syncable::MutableEntry *trans);

}  
}  

#endif  
