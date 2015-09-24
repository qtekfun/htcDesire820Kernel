// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_TEST_ENGINE_TEST_DIRECTORY_SETTER_UPPER_H_
#define SYNC_TEST_ENGINE_TEST_DIRECTORY_SETTER_UPPER_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "sync/test/fake_sync_encryption_handler.h"
#include "sync/test/null_directory_change_delegate.h"
#include "sync/util/test_unrecoverable_error_handler.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace syncer {

namespace syncable {
class Directory;
class DirectoryBackingStore;
class TestTransactionObserver;
}

class TestDirectorySetterUpper {
 public:
  TestDirectorySetterUpper();
  virtual ~TestDirectorySetterUpper();

  
  virtual void SetUp();

  
  
  virtual void SetUpWith(
      syncer::syncable::DirectoryBackingStore* directory_store);

  
  
  
  virtual void TearDown();

  syncable::Directory* directory() { return directory_.get(); }

  SyncEncryptionHandler* encryption_handler() { return &encryption_handler_; }

  syncable::TestTransactionObserver* transaction_observer() {
    return test_transaction_observer_.get();
  }

 private:
  syncable::NullDirectoryChangeDelegate delegate_;
  scoped_ptr<syncable::TestTransactionObserver> test_transaction_observer_;
  TestUnrecoverableErrorHandler handler_;

  void RunInvariantCheck();

  FakeSyncEncryptionHandler encryption_handler_;
  scoped_ptr<syncable::Directory> directory_;
  std::string name_;

  DISALLOW_COPY_AND_ASSIGN(TestDirectorySetterUpper);
};

}  

#endif  
