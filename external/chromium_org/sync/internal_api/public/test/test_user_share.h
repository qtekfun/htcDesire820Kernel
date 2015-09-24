// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_PUBLIC_TEST_TEST_USER_SHARE_H_
#define SYNC_INTERNAL_API_PUBLIC_TEST_TEST_USER_SHARE_H_

#include "base/basictypes.h"
#include "sync/internal_api/public/base/model_type.h"
#include "sync/internal_api/public/user_share.h"

namespace syncer {

class SyncEncryptionHandler;
class TestDirectorySetterUpper;

namespace syncable {
  class TestTransactionObserver;
}

class TestUserShare {
 public:
  TestUserShare();
  ~TestUserShare();

  
  
  void SetUp();

  
  
  
  
  void TearDown();

  
  bool Reload();

  
  UserShare* user_share();

  
  
  SyncEncryptionHandler* encryption_handler();

  
  
  syncable::TestTransactionObserver* transaction_observer();

  
  static bool CreateRoot(syncer::ModelType model_type,
                         syncer::UserShare* service);

  size_t GetDeleteJournalSize() const;

 private:
  scoped_ptr<TestDirectorySetterUpper> dir_maker_;
  scoped_ptr<UserShare> user_share_;

  DISALLOW_COPY_AND_ASSIGN(TestUserShare);
};

}  

#endif  
