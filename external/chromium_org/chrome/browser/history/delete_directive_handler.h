// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_DELETE_DIRECTIVE_HANDLER_H_
#define CHROME_BROWSER_HISTORY_DELETE_DIRECTIVE_HANDLER_H_

#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/thread_checker.h"
#include "chrome/browser/common/cancelable_request.h"
#include "sync/api/sync_change_processor.h"
#include "sync/api/sync_data.h"

namespace sync_pb {
class HistoryDeleteDirectiveSpecifics;
}

class HistoryService;

namespace history {

class DeleteDirectiveHandler {
 public:
  DeleteDirectiveHandler();
  ~DeleteDirectiveHandler();

  
  void Start(HistoryService* history_service,
             const syncer::SyncDataList& initial_sync_data,
             scoped_ptr<syncer::SyncChangeProcessor> sync_processor);
  void Stop();

  
  
  
  bool CreateDeleteDirectives(
      const std::set<int64>& global_ids,
      base::Time begin_time,
      base::Time end_time);

  
  
  
  
  
  
  syncer::SyncError ProcessLocalDeleteDirective(
      const sync_pb::HistoryDeleteDirectiveSpecifics& delete_directive);

  
  syncer::SyncError ProcessSyncChanges(
      HistoryService* history_service,
      const syncer::SyncChangeList& change_list);

 private:
  class DeleteDirectiveTask;
  friend class DeleteDirectiveTask;

  
  enum PostProcessingAction {
    KEEP_AFTER_PROCESSING,
    DROP_AFTER_PROCESSING
  };

  
  
  void FinishProcessing(PostProcessingAction post_processing_action,
                        const syncer::SyncDataList& delete_directives);

  CancelableRequestConsumer internal_consumer_;
  scoped_ptr<syncer::SyncChangeProcessor> sync_processor_;
  base::ThreadChecker thread_checker_;
  base::WeakPtrFactory<DeleteDirectiveHandler> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(DeleteDirectiveHandler);
};

}  

#endif  
