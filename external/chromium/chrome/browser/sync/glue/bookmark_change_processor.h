// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_GLUE_BOOKMARK_CHANGE_PROCESSOR_H_
#define CHROME_BROWSER_SYNC_GLUE_BOOKMARK_CHANGE_PROCESSOR_H_
#pragma once

#include <vector>

#include "chrome/browser/bookmarks/bookmark_model.h"
#include "chrome/browser/sync/engine/syncapi.h"
#include "chrome/browser/sync/glue/change_processor.h"
#include "chrome/browser/sync/glue/bookmark_model_associator.h"
#include "chrome/browser/sync/glue/sync_backend_host.h"

namespace browser_sync {

class BookmarkChangeProcessor : public BookmarkModelObserver,
                                public ChangeProcessor {
 public:
  BookmarkChangeProcessor(BookmarkModelAssociator* model_associator,
                          UnrecoverableErrorHandler* error_handler);
  virtual ~BookmarkChangeProcessor() {}

  
  
  virtual void Loaded(BookmarkModel* model);
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model);
  virtual void BookmarkNodeMoved(BookmarkModel* model,
                                 const BookmarkNode* old_parent,
                                 int old_index,
                                 const BookmarkNode* new_parent,
                                 int new_index);
  virtual void BookmarkNodeAdded(BookmarkModel* model,
                                 const BookmarkNode* parent,
                                 int index);
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int index,
                                   const BookmarkNode* node);
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node);
  virtual void BookmarkNodeFaviconLoaded(BookmarkModel* model,
                                         const BookmarkNode* node);
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node);

  
  
  virtual void ApplyChangesFromSyncModel(
      const sync_api::BaseTransaction* trans,
      const sync_api::SyncManager::ChangeRecord* changes,
      int change_count);

  
  
  
  
  static const BookmarkNode* CreateBookmarkNode(
      sync_api::BaseNode* sync_node,
      const BookmarkNode* parent,
      BookmarkModel* model,
      int index);

  
  
  
  
  static bool SetBookmarkFavicon(sync_api::BaseNode* sync_node,
                                 const BookmarkNode* bookmark_node,
                                 BookmarkModel* model);

  
  
  
  static void ApplyBookmarkFavicon(
      const BookmarkNode* bookmark_node,
      Profile* profile,
      const std::vector<unsigned char>& icon_bytes_vector);

  
  static void SetSyncNodeFavicon(const BookmarkNode* bookmark_node,
                                 BookmarkModel* model,
                                 sync_api::WriteNode* sync_node);

  
  
  
  
  
  static int64 CreateSyncNode(const BookmarkNode* parent,
                              BookmarkModel* model,
                              int index,
                              sync_api::WriteTransaction* trans,
                              BookmarkModelAssociator* associator,
                              UnrecoverableErrorHandler* error_handler);

 protected:
  virtual void StartImpl(Profile* profile);
  virtual void StopImpl();

 private:
  enum MoveOrCreate {
    MOVE,
    CREATE,
  };

  
  
  const BookmarkNode* CreateOrUpdateBookmarkNode(
      sync_api::BaseNode* src,
      BookmarkModel* model);

  
  
  
  
  
  int CalculateBookmarkModelInsertionIndex(
      const BookmarkNode* parent,
      const sync_api::BaseNode* node) const;

  
  
  
  
  
  
  
  
  static bool PlaceSyncNode(MoveOrCreate operation,
                            const BookmarkNode* parent,
                            int index,
                            sync_api::WriteTransaction* trans,
                            sync_api::WriteNode* dst,
                            BookmarkModelAssociator* associator);

  
  static void UpdateSyncNodeProperties(const BookmarkNode* src,
                                       BookmarkModel* model,
                                       sync_api::WriteNode* dst);

  
  static void EncodeFavicon(const BookmarkNode* src,
                            BookmarkModel* model,
                            std::vector<unsigned char>* dst);

  
  
  void RemoveOneSyncNode(sync_api::WriteTransaction* trans,
                         const BookmarkNode* node);

  
  void RemoveSyncNodeHierarchy(const BookmarkNode* node);

  
  
  BookmarkModel* bookmark_model_;

  
  BookmarkModelAssociator* model_associator_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkChangeProcessor);
};

}  

#endif  
