// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_INDEXED_DB_INDEXED_DB_TRANSACTION_COORDINATOR_H_
#define CONTENT_BROWSER_INDEXED_DB_INDEXED_DB_TRANSACTION_COORDINATOR_H_

#include <map>
#include <set>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/browser/indexed_db/list_set.h"

namespace content {

class IndexedDBTransaction;

class IndexedDBTransactionCoordinator {
 public:
  IndexedDBTransactionCoordinator();
  ~IndexedDBTransactionCoordinator();

  
  void DidCreateTransaction(scoped_refptr<IndexedDBTransaction> transaction);
  void DidFinishTransaction(scoped_refptr<IndexedDBTransaction> transaction);

#ifndef NDEBUG
  bool IsActive(IndexedDBTransaction* transaction);
#endif

  
  std::vector<const IndexedDBTransaction*> GetTransactions() const;

 private:
  void ProcessQueuedTransactions();
  bool CanStartTransaction(IndexedDBTransaction* const transaction,
                           const std::set<int64>& locked_scope) const;

  
  
  
  typedef list_set<scoped_refptr<IndexedDBTransaction> > TransactionSet;
  TransactionSet queued_transactions_;
  TransactionSet started_transactions_;
};

}  

#endif  
