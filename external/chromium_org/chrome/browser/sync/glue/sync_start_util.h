// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_SYNC_START_UTIL_H_
#define CHROME_BROWSER_SYNC_GLUE_SYNC_START_UTIL_H_

#include "sync/api/syncable_service.h"

namespace base {
class FilePath;
}

namespace sync_start_util {

syncer::SyncableService::StartSyncFlare GetFlareForSyncableService(
    const base::FilePath& profile_path);

}  

#endif  
