// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_BOOKMARK_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_GLUE_BOOKMARK_CHANGE_PROCESSOR_H_

#include <vector>

#include "base/compiler_specific.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"
#include "chrome/browser/sync/glue/bookmark_model_associator.h"
#include "chrome/browser/sync/glue/change_processor.h"
#include "chrome/browser/sync/glue/data_type_error_handler.h"
#include "chrome/browser/sync/glue/sync_backend_host.h"

namespace base {
class RefCountedMemory;
}

namespace syncer {
class WriteNode;
class WriteTransaction;
}  

namespace browser_sync {

class BookmarkChangeProcessor : public BookmarkModelObserver,
                                public ChangeProcessor {
 public:
  BookmarkChangeProcessor(BookmarkModelAssociator* model_associator,
                          DataTypeErrorHandler* error_handler);
  virtual ~BookmarkChangeProcessor();

  
  
  virtual void Loaded(BookmarkModel* model, bool ids_reassigned) OVERRIDE;
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkNodeMoved(BookmarkModel* model,
                                 const BookmarkNode* old_parent,
                                 int old_index,
                                 const BookmarkNode* new_parent,
                                 int new_index) OVERRIDE;
  virtual void BookmarkNodeAdded(BookmarkModel* model,
                                 const BookmarkNode* parent,
                                 int index) OVERRIDE;
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int index,
                                   const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkAllNodesRemoved(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkMetaInfoChanged(BookmarkModel* model,
                                       const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeFaviconChanged(BookmarkModel* model,
                                          const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node) OVERRIDE;

  
  
  virtual void ApplyChangesFromSyncModel(
      const syncer::BaseTransaction* trans,
      int64 model_version,
      const syncer::ImmutableChangeRecordList& changes) OVERRIDE;

  
  

  
  
  static void UpdateBookmarkWithSyncData(
      const syncer::BaseNode& sync_node,
      BookmarkModel* model,
      const BookmarkNode* node,
      Profile* profile);

  
  
  
  static const BookmarkNode* CreateBookmarkNode(
      syncer::BaseNode* sync_node,
      const BookmarkNode* parent,
      BookmarkModel* model,
      Profile* profile,
      int index);

  
  
  
  
  static bool SetBookmarkFavicon(const syncer::BaseNode* sync_node,
                                 const BookmarkNode* bookmark_node,
                                 BookmarkModel* model,
                                 Profile* profile);

  
  
  
  static void ApplyBookmarkFavicon(
      const BookmarkNode* bookmark_node,
      Profile* profile,
      const GURL& icon_url,
      const scoped_refptr<base::RefCountedMemory>& bitmap_data);

  
  static void SetSyncNodeFavicon(const BookmarkNode* bookmark_node,
                                 BookmarkModel* model,
                                 syncer::WriteNode* sync_node);

  
  
  
  
  
  static int64 CreateSyncNode(const BookmarkNode* parent,
                              BookmarkModel* model,
                              int index,
                              syncer::WriteTransaction* trans,
                              BookmarkModelAssociator* associator,
                              DataTypeErrorHandler* error_handler);

  
  
  static void UpdateTransactionVersion(
      int64 new_version,
      BookmarkModel* model,
      const std::vector<const BookmarkNode*>& nodes);

 protected:
  virtual void StartImpl(Profile* profile) OVERRIDE;

 private:
  enum MoveOrCreate {
    MOVE,
    CREATE,
  };

  
  static void SetBookmarkMetaInfo(const syncer::BaseNode* sync_node,
                                  const BookmarkNode* bookmark_node,
                                  BookmarkModel* bookmark_model);

  
  static void SetSyncNodeMetaInfo(const BookmarkNode* node,
                                  syncer::WriteNode* sync_node);

  
  
  
  
  
  
  
  
  static bool PlaceSyncNode(MoveOrCreate operation,
                            const BookmarkNode* parent,
                            int index,
                            syncer::WriteTransaction* trans,
                            syncer::WriteNode* dst,
                            BookmarkModelAssociator* associator);

  
  static void UpdateSyncNodeProperties(const BookmarkNode* src,
                                       BookmarkModel* model,
                                       syncer::WriteNode* dst);

  
  static void EncodeFavicon(const BookmarkNode* src,
                            BookmarkModel* model,
                            scoped_refptr<base::RefCountedMemory>* dst);

  
  void RemoveOneSyncNode(syncer::WriteNode* sync_node);

  
  void RemoveAllSyncNodes();

  
  
  void RemoveAllChildNodes(syncer::WriteTransaction* trans,
                           const int64& topmost_node_id);

  
  void RemoveSyncNodeHierarchy(const BookmarkNode* node);

  
  
  BookmarkModel* bookmark_model_;

  Profile* profile_;

  
  BookmarkModelAssociator* model_associator_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkChangeProcessor);
};

}  

#endif  
