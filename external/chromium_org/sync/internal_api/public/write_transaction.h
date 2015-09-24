// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_WRITE_TRANSACTION_H_
#define SYNC_INTERNAL_API_PUBLIC_WRITE_TRANSACTION_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base_transaction.h"

namespace tracked_objects {
class Location;
}  

namespace syncer {

namespace syncable {
class BaseTransaction;
class WriteTransaction;
}  

class SYNC_EXPORT WriteTransaction : public BaseTransaction {
 public:
  
  WriteTransaction(const tracked_objects::Location& from_here,
                   UserShare* share);
  
  
  
  
  WriteTransaction(const tracked_objects::Location& from_here,
                   UserShare* share, int64* transaction_version);
  virtual ~WriteTransaction();

  
  virtual syncable::BaseTransaction* GetWrappedTrans() const OVERRIDE;
  syncable::WriteTransaction* GetWrappedWriteTrans() { return transaction_; }

 protected:
  WriteTransaction() {}

  void SetTransaction(syncable::WriteTransaction* trans) {
    transaction_ = trans;
  }

 private:
  void* operator new(size_t size);  

  
  syncable::WriteTransaction* transaction_;

  DISALLOW_COPY_AND_ASSIGN(WriteTransaction);
};

}  

#endif  
