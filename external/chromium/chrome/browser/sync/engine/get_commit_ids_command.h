// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_GET_COMMIT_IDS_COMMAND_H_
#define CHROME_BROWSER_SYNC_ENGINE_GET_COMMIT_IDS_COMMAND_H_
#pragma once

#include <utility>
#include <vector>

#include "chrome/browser/sync/engine/syncer_command.h"
#include "chrome/browser/sync/engine/syncer_util.h"
#include "chrome/browser/sync/sessions/ordered_commit_set.h"
#include "chrome/browser/sync/sessions/sync_session.h"

using std::pair;
using std::vector;

namespace browser_sync {

class GetCommitIdsCommand : public SyncerCommand {
  friend class SyncerTest;

 public:
  explicit GetCommitIdsCommand(int commit_batch_size);
  virtual ~GetCommitIdsCommand();

  
  virtual void ExecuteImpl(sessions::SyncSession* session);

  
  void BuildCommitIds(const vector<int64>& unsynced_handles,
                      syncable::WriteTransaction* write_transaction,
                      const ModelSafeRoutingInfo& routes);

  
  
  
  class CommitMetahandleIterator {
   public:
    
    
    CommitMetahandleIterator(const vector<int64>& unsynced_handles,
                             syncable::WriteTransaction* write_transaction,
                             sessions::OrderedCommitSet* commit_set)
        : write_transaction_(write_transaction),
          handle_iterator_(unsynced_handles.begin()),
          unsynced_handles_end_(unsynced_handles.end()),
          commit_set_(commit_set) {

      
      DCHECK(write_transaction_);

      if (Valid() && !ValidateMetahandleForCommit(*handle_iterator_))
        Increment();
    }
    ~CommitMetahandleIterator() {}

    int64 Current() const {
      DCHECK(Valid());
      return *handle_iterator_;
    }

    bool Increment() {
      if (!Valid())
        return false;

      for (++handle_iterator_;
           handle_iterator_ != unsynced_handles_end_;
           ++handle_iterator_) {
        if (ValidateMetahandleForCommit(*handle_iterator_))
          return true;
      }

      return false;
    }

    bool Valid() const {
      return !(handle_iterator_ == unsynced_handles_end_);
    }

    private:
     bool ValidateMetahandleForCommit(int64 metahandle) {
       if (commit_set_->HaveCommitItem(metahandle))
          return false;

       
       
       syncable::MutableEntry entry(write_transaction_,
           syncable::GET_BY_HANDLE, metahandle);
       VerifyCommitResult verify_result =
           SyncerUtil::ValidateCommitEntry(&entry);
       if (verify_result == VERIFY_UNSYNCABLE) {
         
         entry.Put(syncable::IS_UNSYNCED, false);
       }
       return verify_result == VERIFY_OK;
     }

     syncable::WriteTransaction* const write_transaction_;
     vector<int64>::const_iterator handle_iterator_;
     vector<int64>::const_iterator unsynced_handles_end_;
     sessions::OrderedCommitSet* commit_set_;

     DISALLOW_COPY_AND_ASSIGN(CommitMetahandleIterator);
  };

 private:
  void AddUncommittedParentsAndTheirPredecessors(
      syncable::BaseTransaction* trans,
      syncable::Id parent_id,
      const ModelSafeRoutingInfo& routes);

  
  
  
  bool AddItem(syncable::Entry* item, sessions::OrderedCommitSet* result);
  bool AddItemThenPredecessors(syncable::BaseTransaction* trans,
                               syncable::Entry* item,
                               syncable::IndexedBitField inclusion_filter,
                               sessions::OrderedCommitSet* result);
  void AddPredecessorsThenItem(syncable::BaseTransaction* trans,
                               syncable::Entry* item,
                               syncable::IndexedBitField inclusion_filter,
                               const ModelSafeRoutingInfo& routes);

  bool IsCommitBatchFull();

  void AddCreatesAndMoves(const vector<int64>& unsynced_handles,
                          syncable::WriteTransaction* write_transaction,
                          const ModelSafeRoutingInfo& routes);

  void AddDeletes(const vector<int64>& unsynced_handles,
                  syncable::WriteTransaction* write_transaction);

  scoped_ptr<sessions::OrderedCommitSet> ordered_commit_set_;

  int requested_commit_batch_size_;

  DISALLOW_COPY_AND_ASSIGN(GetCommitIdsCommand);
};

}  

#endif  
