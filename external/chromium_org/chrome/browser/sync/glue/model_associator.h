// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_MODEL_ASSOCIATOR_H_
#define CHROME_BROWSER_SYNC_GLUE_MODEL_ASSOCIATOR_H_

#include "base/basictypes.h"
#include "base/synchronization/lock.h"
#include "sync/api/sync_error.h"
#include "sync/internal_api/public/base/model_type.h"

namespace syncer {
class BaseNode;
class SyncMergeResult;
}

namespace browser_sync {

class AssociatorInterface {
 public:
  virtual ~AssociatorInterface() {}

  
  
  
  
  
  virtual syncer::SyncError AssociateModels(
      syncer::SyncMergeResult* local_merge_result,
      syncer::SyncMergeResult* syncer_merge_result) = 0;

  
  virtual syncer::SyncError DisassociateModels() = 0;

  
  
  
  virtual bool SyncModelHasUserCreatedNodes(bool* has_nodes) = 0;

  
  
  
  
  
  virtual void AbortAssociation() = 0;

  
  
  
  
  virtual bool CryptoReadyIfNecessary() = 0;
};

template <class Node, class IDType>
class PerDataTypeAssociatorInterface : public AssociatorInterface {
 public:
  virtual ~PerDataTypeAssociatorInterface() {}
  
  
  
  virtual int64 GetSyncIdFromChromeId(const IDType& id) = 0;

  
  
  virtual const Node* GetChromeNodeFromSyncId(int64 sync_id) = 0;

  
  
  
  virtual bool InitSyncNodeFromChromeId(
      const IDType& node_id,
      syncer::BaseNode* sync_node) = 0;

  
  virtual void Associate(const Node* node, int64 sync_id) = 0;

  
  virtual void Disassociate(int64 sync_id) = 0;
};

}  

#endif  
