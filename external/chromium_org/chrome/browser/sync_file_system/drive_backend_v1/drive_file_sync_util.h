// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_V1_DRIVE_FILE_SYNC_UTIL_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_DRIVE_BACKEND_V1_DRIVE_FILE_SYNC_UTIL_H_

#include "chrome/browser/sync_file_system/sync_status_code.h"
#include "google_apis/drive/gdata_errorcode.h"

namespace sync_file_system {

SyncStatusCode GDataErrorCodeToSyncStatusCode(
    google_apis::GDataErrorCode error);

void SetDisableDriveAPI(bool flag);

bool IsDriveAPIDisabled();

class ScopedDisableDriveAPI {
 public:
  ScopedDisableDriveAPI();
  ~ScopedDisableDriveAPI();

 private:
  bool was_disabled_;

  DISALLOW_COPY_AND_ASSIGN(ScopedDisableDriveAPI);
};

}  

#endif  
