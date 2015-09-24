// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_FILE_SYSTEM_SYNC_PROCESS_RUNNER_H_
#define CHROME_BROWSER_SYNC_FILE_SYSTEM_SYNC_PROCESS_RUNNER_H_

#include "base/basictypes.h"
#include "base/memory/weak_ptr.h"
#include "base/timer/timer.h"
#include "chrome/browser/sync_file_system/sync_callbacks.h"
#include "chrome/browser/sync_file_system/sync_service_state.h"

namespace sync_file_system {

class SyncFileSystemService;

class SyncProcessRunner {
 public:
  typedef base::Callback<void(const SyncStatusCallback&)> Task;
  SyncProcessRunner(const std::string& name,
                    SyncFileSystemService* sync_service);
  virtual ~SyncProcessRunner();

  
  virtual void StartSync(const SyncStatusCallback& callback) = 0;

  
  void Schedule();
  void ScheduleIfNotRunning();

 protected:
  void OnChangesUpdated(int64 pending_changes);
  SyncFileSystemService* sync_service() { return sync_service_; }

  
  
  virtual SyncServiceState GetServiceState();

 private:
  void Finished(SyncStatusCode status);
  void Run();
  void ScheduleInternal(int64 delay);

  std::string name_;
  SyncFileSystemService* sync_service_;
  base::OneShotTimer<SyncProcessRunner> timer_;
  base::Time last_scheduled_;
  int64 current_delay_;
  int64 last_delay_;
  int64 pending_changes_;
  bool running_;
  base::WeakPtrFactory<SyncProcessRunner> factory_;

  DISALLOW_COPY_AND_ASSIGN(SyncProcessRunner);
};

}  

#endif  