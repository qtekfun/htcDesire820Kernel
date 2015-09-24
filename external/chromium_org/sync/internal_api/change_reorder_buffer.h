// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SYNC_INTERNAL_API_CHANGE_REORDER_BUFFER_H_
#define SYNC_INTERNAL_API_CHANGE_REORDER_BUFFER_H_

#include <map>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/linked_ptr.h"
#include "sync/internal_api/public/change_record.h"
#include "sync/protocol/sync.pb.h"

namespace syncer {

class BaseTransaction;

class ChangeReorderBuffer {
 public:
  ChangeReorderBuffer();
  ~ChangeReorderBuffer();

  
  
  void PushAddedItem(int64 id);

  
  
  void PushDeletedItem(int64 id);

  
  
  void PushUpdatedItem(int64 id);

  void SetExtraDataForId(int64 id, ExtraPasswordChangeRecordData* extra);

  void SetSpecificsForId(int64 id, const sync_pb::EntitySpecifics& specifics);

  
  
  void Clear();

  bool IsEmpty() const;

  
  
  
  
  bool GetAllChangesInTreeOrder(
      const BaseTransaction* sync_trans,
      ImmutableChangeRecordList* changes) WARN_UNUSED_RESULT;

 private:
  class Traversal;
  typedef std::map<int64, ChangeRecord::Action> OperationMap;
  typedef std::map<int64, sync_pb::EntitySpecifics> SpecificsMap;
  typedef std::map<int64, linked_ptr<ExtraPasswordChangeRecordData> >
      ExtraDataMap;

  
  
  OperationMap operations_;

  
  SpecificsMap specifics_;

  
  ExtraDataMap extra_data_;

  DISALLOW_COPY_AND_ASSIGN(ChangeReorderBuffer);
};

}  

#endif  
