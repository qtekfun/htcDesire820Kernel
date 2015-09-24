// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_SYNC_EVENT_OBSERVER_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_SYNC_EVENT_OBSERVER_H_

#include <string>

#include "chrome/browser/sync_file_system/file_status_observer.h"
#include "chrome/browser/sync_file_system/sync_service_state.h"

class GURL;

namespace fileapi {
class FileSystemURL;
}

namespace sync_file_system {

class SyncEventObserver {
 public:
  SyncEventObserver() {}
  virtual ~SyncEventObserver() {}

  
  
  virtual void OnSyncStateUpdated(const GURL& app_origin,
                                  SyncServiceState state,
                                  const std::string& description) = 0;

  
  
  virtual void OnFileSynced(const fileapi::FileSystemURL& url,
                            SyncFileStatus status,
                            SyncAction action,
                            SyncDirection direction) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(SyncEventObserver);
};

}  

#endif  
