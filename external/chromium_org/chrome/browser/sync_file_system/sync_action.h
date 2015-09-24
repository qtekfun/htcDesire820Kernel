// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_SYNC_ACTION_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_SYNC_ACTION_H_

namespace sync_file_system {

enum SyncAction {
  
  SYNC_ACTION_NONE,

  
  SYNC_ACTION_ADDED,

  
  SYNC_ACTION_UPDATED,

  
  SYNC_ACTION_DELETED,
};

const char* SyncActionToString(SyncAction action);

}  

#endif  
