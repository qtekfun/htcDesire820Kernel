// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_BOOKMARK_MODEL_ASSOCIATOR_H_
#define CHROME_BROWSER_SYNC_GLUE_BOOKMARK_MODEL_ASSOCIATOR_H_
#pragma once

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/task.h"
#include "chrome/browser/sync/unrecoverable_error_handler.h"
#include "chrome/browser/sync/glue/model_associator.h"

class BookmarkModel;
class BookmarkNode;

namespace sync_api {
class BaseNode;
class BaseTransaction;
class ReadNode;
struct UserShare;
}

namespace browser_sync {

class BookmarkChangeProcessor;

class BookmarkModelAssociator
    : public PerDataTypeAssociatorInterface<BookmarkNode, int64> {
 public:
  static syncable::ModelType model_type() { return syncable::BOOKMARKS; }
  BookmarkModelAssociator(
      BookmarkModel* bookmark_model,
      sync_api::UserShare* user_share,
      UnrecoverableErrorHandler* unrecoverable_error_handler);
  virtual ~BookmarkModelAssociator();

  
  
  
  
  
  
  
  
  
  virtual bool AssociateModels();

  virtual bool DisassociateModels();

  
  
  virtual bool SyncModelHasUserCreatedNodes(bool* has_nodes);

  
  
  
  virtual int64 GetSyncIdFromChromeId(const int64& node_id);

  
  
  virtual const BookmarkNode* GetChromeNodeFromSyncId(int64 sync_id);

  
  
  
  virtual bool InitSyncNodeFromChromeId(const int64& node_id,
                                        sync_api::BaseNode* sync_node);

  
  virtual void Associate(const BookmarkNode* node, int64 sync_id);
  
  virtual void Disassociate(int64 sync_id);

  virtual void AbortAssociation() {
    
    
  }

  
  virtual bool CryptoReadyIfNecessary();

 protected:
  
  
  
  virtual bool GetSyncIdForTaggedNode(const std::string& tag, int64* sync_id);

 private:
  typedef std::map<int64, int64> BookmarkIdToSyncIdMap;
  typedef std::map<int64, const BookmarkNode*> SyncIdToBookmarkNodeMap;
  typedef std::set<int64> DirtyAssociationsSyncIds;

  
  void PostPersistAssociationsTask();
  
  void PersistAssociations();

  
  
  
  bool LoadAssociations();

  
  
  bool BuildAssociations();

  
  
  
  
  
  bool AssociateTaggedPermanentNode(const BookmarkNode* permanent_node,
                                    const std::string& tag);

  
  bool NodesMatch(const BookmarkNode* bookmark,
                  const sync_api::BaseNode* sync_node) const;

  BookmarkModel* bookmark_model_;
  sync_api::UserShare* user_share_;
  UnrecoverableErrorHandler* unrecoverable_error_handler_;
  BookmarkIdToSyncIdMap id_map_;
  SyncIdToBookmarkNodeMap id_map_inverse_;
  
  DirtyAssociationsSyncIds dirty_associations_sync_ids_;

  
  
  
  ScopedRunnableMethodFactory<BookmarkModelAssociator> persist_associations_;

  int number_of_new_sync_nodes_created_at_association_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkModelAssociator);
};

}  

#endif  
