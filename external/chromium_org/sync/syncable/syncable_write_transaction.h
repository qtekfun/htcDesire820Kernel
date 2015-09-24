// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SYNCABLE_SYNCABLE_WRITE_TRANSACTION_H_
#define SYNC_SYNCABLE_SYNCABLE_WRITE_TRANSACTION_H_

#include "sync/base/sync_export.h"
#include "sync/syncable/entry_kernel.h"
#include "sync/syncable/syncable_base_write_transaction.h"

namespace syncer {
namespace syncable {

SYNC_EXPORT extern const int64 kInvalidTransactionVersion;

class SYNC_EXPORT WriteTransaction : public BaseWriteTransaction {
 public:
  WriteTransaction(const tracked_objects::Location& from_here,
                   WriterTag writer, Directory* directory);

  
  
  
  
  
  WriteTransaction(const tracked_objects::Location& from_here,
                   Directory* directory, int64* transaction_version);

  virtual ~WriteTransaction();

  virtual void TrackChangesTo(const EntryKernel* entry) OVERRIDE;

 protected:
  
  virtual void NotifyTransactionComplete(ModelTypeSet models_with_changes);

 private:
  friend class MutableEntry;

  
  ImmutableEntryKernelMutationMap RecordMutations();

  void UnlockAndNotify(const ImmutableEntryKernelMutationMap& mutations);

  ModelTypeSet NotifyTransactionChangingAndEnding(
      const ImmutableEntryKernelMutationMap& mutations);

  
  
  void UpdateTransactionVersion(const std::vector<int64>& entry_changed);

  
  
  EntryKernelMutationMap mutations_;

  
  
  int64* transaction_version_;

  DISALLOW_COPY_AND_ASSIGN(WriteTransaction);
};

}  
}  

#endif 
