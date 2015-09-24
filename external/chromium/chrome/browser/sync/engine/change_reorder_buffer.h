// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_ENGINE_CHANGE_REORDER_BUFFER_H_
#define CHROME_BROWSER_SYNC_ENGINE_CHANGE_REORDER_BUFFER_H_
#pragma once

#include <map>
#include <vector>

#include "base/memory/linked_ptr.h"
#include "chrome/browser/sync/engine/syncapi.h"
#include "chrome/browser/sync/protocol/sync.pb.h"

namespace sync_api {

class ChangeReorderBuffer {
 public:
  typedef SyncManager::ChangeRecord ChangeRecord;
  typedef SyncManager::ExtraPasswordChangeRecordData ExtraChangeRecordData;

  ChangeReorderBuffer();
  ~ChangeReorderBuffer();

  
  
  
  void PushAddedItem(int64 id) {
    operations_[id] = OP_ADD;
  }

  
  
  
  void PushDeletedItem(int64 id) {
    operations_[id] = OP_DELETE;
  }

  
  
  
  
  
  void PushUpdatedItem(int64 id, bool position_changed) {
    operations_[id] = position_changed ? OP_UPDATE_POSITION_AND_PROPERTIES :
                                         OP_UPDATE_PROPERTIES_ONLY;
  }

  void SetExtraDataForId(int64 id, ExtraChangeRecordData* extra) {
    extra_data_[id] = make_linked_ptr<ExtraChangeRecordData>(extra);
  }

  void SetSpecificsForId(int64 id, const sync_pb::EntitySpecifics& specifics) {
    specifics_[id] = specifics;
  }

  
  
  void Clear() {
    operations_.clear();
  }

  bool IsEmpty() const {
    return operations_.empty();
  }

  
  
  
  void GetAllChangesInTreeOrder(const BaseTransaction* sync_trans,
                                std::vector<ChangeRecord>* changelist);

 private:
  class Traversal;
  enum Operation {
    OP_ADD,                             
    OP_DELETE,                          
    OP_UPDATE_PROPERTIES_ONLY,          
    OP_UPDATE_POSITION_AND_PROPERTIES,  
  };
  typedef std::map<int64, Operation> OperationMap;
  typedef std::map<int64, sync_pb::EntitySpecifics> SpecificsMap;
  typedef std::map<int64, linked_ptr<ExtraChangeRecordData> > ExtraDataMap;

  
  
  OperationMap operations_;

  
  SpecificsMap specifics_;

  
  ExtraDataMap extra_data_;

  DISALLOW_COPY_AND_ASSIGN(ChangeReorderBuffer);
};

}  

#endif  
