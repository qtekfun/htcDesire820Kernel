// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_TEST_NULL_TRANSACTION_OBSERVER_H_
#define SYNC_TEST_NULL_TRANSACTION_OBSERVER_H_

#include "sync/internal_api/public/util/weak_handle.h"

namespace syncer {
namespace syncable {

class TransactionObserver;

WeakHandle<TransactionObserver> NullTransactionObserver();

}  
}  

#endif  