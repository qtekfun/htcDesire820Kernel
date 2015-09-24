// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_SYNCABLE_SYNCABLE_READ_TRANSACTION_H_
#define SYNC_SYNCABLE_SYNCABLE_READ_TRANSACTION_H_

#include "sync/base/sync_export.h"
#include "sync/syncable/syncable_base_transaction.h"

namespace syncer {
class ReadTransaction;
namespace syncable {

class SYNC_EXPORT ReadTransaction : public BaseTransaction {
 public:
  ReadTransaction(const tracked_objects::Location& from_here,
                  Directory* directory);

  virtual ~ReadTransaction();

 protected:  
  friend class syncer::ReadTransaction;
  void* operator new(size_t size) { return (::operator new)(size); }

  DISALLOW_COPY_AND_ASSIGN(ReadTransaction);
};

}  
}  

#endif  
