// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_MODEL_OBSERVER_FOR_COCOA_H
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_MODEL_OBSERVER_FOR_COCOA_H

#import <Cocoa/Cocoa.h>

#include <set>

#include "base/basictypes.h"
#include "base/mac/scoped_block.h"
#include "chrome/browser/bookmarks/bookmark_model.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"

class BookmarkModelObserverForCocoa : public BookmarkModelObserver {
 public:
  
  
  typedef void(^ChangeCallback)(BOOL nodeWasDeleted);

  
  
  BookmarkModelObserverForCocoa(BookmarkModel* model,
                                ChangeCallback callback);
  virtual ~BookmarkModelObserverForCocoa();

  
  
  void StartObservingNode(const BookmarkNode* node);
  void StopObservingNode(const BookmarkNode* node);

  
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkNodeMoved(BookmarkModel* model,
                                 const BookmarkNode* old_parent,
                                 int old_index,
                                 const BookmarkNode* new_parent,
                                 int new_index) OVERRIDE;
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int old_index,
                                   const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkAllNodesRemoved(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node) OVERRIDE;

  
  

  virtual void Loaded(BookmarkModel* model, bool ids_reassigned) OVERRIDE { }
  virtual void BookmarkNodeAdded(BookmarkModel* model,
                                 const BookmarkNode* parent,
                                 int index) OVERRIDE { }
  virtual void BookmarkNodeFaviconChanged(BookmarkModel* model,
                                          const BookmarkNode* node) OVERRIDE { }
  virtual void BookmarkNodeChildrenReordered(
      BookmarkModel* model,
      const BookmarkNode* node) OVERRIDE { }

  virtual void ExtensiveBookmarkChangesBeginning(
      BookmarkModel* model) OVERRIDE { }

  virtual void ExtensiveBookmarkChangesEnded(BookmarkModel* model) OVERRIDE { }

 private:
  BookmarkModel* model_;  
  std::set<const BookmarkNode*> nodes_;  
  base::mac::ScopedBlock<ChangeCallback> callback_;

  
  
  void Notify(BOOL deleted);

  DISALLOW_COPY_AND_ASSIGN(BookmarkModelObserverForCocoa);
};

#endif  
