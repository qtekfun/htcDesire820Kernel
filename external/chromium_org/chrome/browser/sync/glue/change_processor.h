// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_GLUE_CHANGE_PROCESSOR_H_

#include "chrome/browser/sync/glue/data_type_error_handler.h"
#include "chrome/browser/sync/glue/sync_backend_host.h"
#include "sync/internal_api/public/change_record.h"

class Profile;

namespace syncer {
class UnrecoverableErrorHandler;
}  

namespace browser_sync {

class ModelAssociator;

class ChangeProcessor {
 public:
  explicit ChangeProcessor(DataTypeErrorHandler* error_handler);
  virtual ~ChangeProcessor();

  
  
  
  
  
  
  
  void Start(Profile* profile, syncer::UserShare* share_handle);

  
  
  
  virtual void ApplyChangesFromSyncModel(
      const syncer::BaseTransaction* trans,
      int64 model_version,
      const syncer::ImmutableChangeRecordList& changes) = 0;

  
  
  
  
  
  
  
  virtual void CommitChangesFromSyncModel();

  
  
  template <class T>
  class ScopedStopObserving {
   public:
    explicit ScopedStopObserving(T* processor)
        : processor_(processor) {
      processor_->StopObserving();
    }
    ~ScopedStopObserving() {
      processor_->StartObserving();
    }

   private:
    ScopedStopObserving() {}
    T* processor_;
  };

 protected:
  
  
  virtual void StartImpl(Profile* profile) = 0;

  DataTypeErrorHandler* error_handler() const;
  virtual syncer::UserShare* share_handle() const;

 private:
  DataTypeErrorHandler* error_handler_;  

  
  syncer::UserShare* share_handle_;

  DISALLOW_COPY_AND_ASSIGN(ChangeProcessor);
};

}  

#endif  
