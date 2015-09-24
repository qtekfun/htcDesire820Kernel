// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_READ_TRANSACTION_H_
#define SYNC_INTERNAL_API_PUBLIC_READ_TRANSACTION_H_

#include "base/compiler_specific.h"
#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base_transaction.h"

namespace tracked_objects {
class Location;
}  

namespace syncer {

struct UserShare;

class SYNC_EXPORT ReadTransaction : public BaseTransaction {
 public:
  
  ReadTransaction(const tracked_objects::Location& from_here,
                  UserShare* share);

  
  ReadTransaction(UserShare* share, syncable::BaseTransaction* trans);

  virtual ~ReadTransaction();

  
  virtual syncable::BaseTransaction* GetWrappedTrans() const OVERRIDE;

  
  
  int64 GetModelVersion(ModelType type);

 private:
  void* operator new(size_t size);  

  
  syncable::BaseTransaction* transaction_;
  bool close_transaction_;

  DISALLOW_COPY_AND_ASSIGN(ReadTransaction);
};

}  

#endif  
