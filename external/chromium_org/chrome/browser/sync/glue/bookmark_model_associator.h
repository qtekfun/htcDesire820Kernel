// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_BOOKMARK_MODEL_ASSOCIATOR_H_
#define CHROME_BROWSER_SYNC_GLUE_BOOKMARK_MODEL_ASSOCIATOR_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/sync/glue/data_type_controller.h"
#include "chrome/browser/sync/glue/data_type_error_handler.h"
#include "chrome/browser/sync/glue/model_associator.h"
#include "sync/internal_api/public/util/unrecoverable_error_handler.h"

class BookmarkModel;
class BookmarkNode;
class Profile;

namespace syncer {
class BaseNode;
class BaseTransaction;
struct UserShare;
}

namespace browser_sync {

class BookmarkModelAssociator
    : public PerDataTypeAssociatorInterface<BookmarkNode, int64> {
 public:
  static syncer::ModelType model_type() { return syncer::BOOKMARKS; }
  
  
  
  BookmarkModelAssociator(
      BookmarkModel* bookmark_model,
      Profile* profile_,
      syncer::UserShare* user_share,
      DataTypeErrorHandler* unrecoverable_error_handler,
      bool expect_mobile_bookmarks_folder);
  virtual ~BookmarkModelAssociator();

  
  void UpdatePermanentNodeVisibility();

  
  
  
  
  
  
  
  
  
  virtual syncer::SyncError AssociateModels(
      syncer::SyncMergeResult* local_merge_result,
      syncer::SyncMergeResult* syncer_merge_result) OVERRIDE;

  virtual syncer::SyncError DisassociateModels() OVERRIDE;

  
  
  virtual bool SyncModelHasUserCreatedNodes(bool* has_nodes) OVERRIDE;

  
  
  
  virtual int64 GetSyncIdFromChromeId(const int64& node_id) OVERRIDE;

  
  
  virtual const BookmarkNode* GetChromeNodeFromSyncId(int64 sync_id) OVERRIDE;

  
  
  
  virtual bool InitSyncNodeFromChromeId(
      const int64& node_id,
      syncer::BaseNode* sync_node) OVERRIDE;

  
  virtual void Associate(const BookmarkNode* node, int64 sync_id) OVERRIDE;
  
  virtual void Disassociate(int64 sync_id) OVERRIDE;

  virtual void AbortAssociation() OVERRIDE {
    
    
  }

  
  virtual bool CryptoReadyIfNecessary() OVERRIDE;

 protected:
  
  
  
  virtual bool GetSyncIdForTaggedNode(const std::string& tag, int64* sync_id);

 private:
  typedef std::map<int64, int64> BookmarkIdToSyncIdMap;
  typedef std::map<int64, const BookmarkNode*> SyncIdToBookmarkNodeMap;
  typedef std::set<int64> DirtyAssociationsSyncIds;

  
  void PostPersistAssociationsTask();
  
  void PersistAssociations();

  
  
  syncer::SyncError BuildAssociations(
      syncer::SyncMergeResult* local_merge_result,
      syncer::SyncMergeResult* syncer_merge_result);

  
  
  int64 ApplyDeletesFromSyncJournal(syncer::BaseTransaction* trans);

  
  
  
  
  
  
  bool AssociateTaggedPermanentNode(
      const BookmarkNode* permanent_node,
      const std::string& tag) WARN_UNUSED_RESULT;

  
  bool NodesMatch(const BookmarkNode* bookmark,
                  const syncer::BaseNode* sync_node) const;

  
  
  
  
  syncer::SyncError CheckModelSyncState(
      syncer::SyncMergeResult* local_merge_result,
      syncer::SyncMergeResult* syncer_merge_result) const;

  BookmarkModel* bookmark_model_;
  Profile* profile_;
  syncer::UserShare* user_share_;
  DataTypeErrorHandler* unrecoverable_error_handler_;
  const bool expect_mobile_bookmarks_folder_;
  BookmarkIdToSyncIdMap id_map_;
  SyncIdToBookmarkNodeMap id_map_inverse_;
  
  DirtyAssociationsSyncIds dirty_associations_sync_ids_;

  
  
  
  base::WeakPtrFactory<BookmarkModelAssociator> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkModelAssociator);
};

}  

#endif  
