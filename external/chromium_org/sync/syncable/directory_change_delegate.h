// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SYNCABLE_DIRECTORY_CHANGE_DELEGATE_H_
#define SYNC_SYNCABLE_DIRECTORY_CHANGE_DELEGATE_H_

#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/syncable/write_transaction_info.h"

namespace syncer {
namespace syncable {

class SYNC_EXPORT_PRIVATE DirectoryChangeDelegate {
 public:
  
  virtual void HandleCalculateChangesChangeEventFromSyncApi(
      const ImmutableWriteTransactionInfo& write_transaction_info,
      BaseTransaction* trans,
      std::vector<int64>* entries_changed) = 0;
  
  virtual void HandleCalculateChangesChangeEventFromSyncer(
      const ImmutableWriteTransactionInfo& write_transaction_info,
      BaseTransaction* trans,
      std::vector<int64>* entries_changed) = 0;
  
  
  virtual ModelTypeSet HandleTransactionEndingChangeEvent(
      const ImmutableWriteTransactionInfo& write_transaction_info,
      BaseTransaction* trans) = 0;
  virtual void HandleTransactionCompleteChangeEvent(
      ModelTypeSet models_with_changes) = 0;
 protected:
  virtual ~DirectoryChangeDelegate() {}
};

}  
}  

#endif  
