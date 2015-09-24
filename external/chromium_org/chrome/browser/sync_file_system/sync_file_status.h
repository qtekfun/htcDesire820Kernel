// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_SYNC_FILE_STATUS_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_SYNC_FILE_STATUS_H_

namespace sync_file_system {

enum SyncFileStatus {
  
  
  SYNC_FILE_STATUS_UNKNOWN,

  
  
  SYNC_FILE_STATUS_SYNCED,

  
  
  SYNC_FILE_STATUS_HAS_PENDING_CHANGES,

  
  SYNC_FILE_STATUS_CONFLICTING,
};

}  

#endif  
