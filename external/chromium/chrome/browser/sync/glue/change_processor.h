// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_GLUE_CHANGE_PROCESSOR_H_
#pragma once

#include "chrome/browser/sync/engine/syncapi.h"
#include "chrome/browser/sync/glue/sync_backend_host.h"

class Profile;

namespace browser_sync {

class ModelAssociator;
class UnrecoverableErrorHandler;

class ChangeProcessor {
 public:
  explicit ChangeProcessor(UnrecoverableErrorHandler* error_handler)
      : running_(false), error_handler_(error_handler), share_handle_(NULL) {}
  virtual ~ChangeProcessor();

  
  
  
  
  
  
  
  
  void Start(Profile* profile, sync_api::UserShare* share_handle);
  void Stop();
  virtual bool IsRunning() const;

  
  
  
  virtual void ApplyChangesFromSyncModel(
      const sync_api::BaseTransaction* trans,
      const sync_api::SyncManager::ChangeRecord* changes,
      int change_count) = 0;

  
  
  
  
  
  virtual void CommitChangesFromSyncModel() { }

 protected:
  
  
  virtual void StartImpl(Profile* profile) = 0;
  virtual void StopImpl() = 0;

  bool running() { return running_; }
  UnrecoverableErrorHandler* error_handler() { return error_handler_; }
  sync_api::UserShare* share_handle() { return share_handle_; }

 private:
  bool running_;  
  UnrecoverableErrorHandler* error_handler_;  

  
  
  sync_api::UserShare* share_handle_;

  DISALLOW_COPY_AND_ASSIGN(ChangeProcessor);
};

}  

#endif  
