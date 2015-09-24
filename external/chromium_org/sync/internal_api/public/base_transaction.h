// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_BASE_TRANSACTION_H_
#define SYNC_INTERNAL_API_PUBLIC_BASE_TRANSACTION_H_

#include "sync/base/sync_export.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/user_share.h"
#include "sync/util/cryptographer.h"

namespace syncer {

namespace syncable {
class BaseTransaction;
class Directory;
}

class SYNC_EXPORT BaseTransaction {
 public:
  
  virtual syncable::BaseTransaction* GetWrappedTrans() const = 0;
  Cryptographer* GetCryptographer() const;
  ModelTypeSet GetEncryptedTypes() const;

  syncable::Directory* GetDirectory() const {
    if (!user_share_) {
      return NULL;
    } else {
      return user_share_->directory.get();
    }
  }

  UserShare* GetUserShare() const {
    return user_share_;
  }

 protected:
  explicit BaseTransaction(UserShare* share);
  virtual ~BaseTransaction();

  BaseTransaction() : user_share_(NULL) { }

 private:
  UserShare* user_share_;

  DISALLOW_COPY_AND_ASSIGN(BaseTransaction);
};

}  

#endif  
