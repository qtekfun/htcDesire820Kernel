// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_MODEL_OBSERVER_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_MODEL_OBSERVER_H_

class BookmarkModel;
class BookmarkNode;

class BookmarkModelObserver {
 public:
  
  
  virtual void Loaded(BookmarkModel* model, bool ids_reassigned) = 0;

  
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model) {}

  
  virtual void BookmarkNodeMoved(BookmarkModel* model,
                                 const BookmarkNode* old_parent,
                                 int old_index,
                                 const BookmarkNode* new_parent,
                                 int new_index) = 0;

  
  virtual void BookmarkNodeAdded(BookmarkModel* model,
                                 const BookmarkNode* parent,
                                 int index) = 0;

  
  
  
  
  virtual void OnWillRemoveBookmarks(BookmarkModel* model,
                                     const BookmarkNode* parent,
                                     int old_index,
                                     const BookmarkNode* node) {}

  
  
  
  
  
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int old_index,
                                   const BookmarkNode* node) = 0;

  
  virtual void OnWillChangeBookmarkNode(BookmarkModel* model,
                                        const BookmarkNode* node) {}

  
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node) = 0;

  
  virtual void OnWillChangeBookmarkMetaInfo(BookmarkModel* model,
                                            const BookmarkNode* node) {}

  
  virtual void BookmarkMetaInfoChanged(BookmarkModel* model,
                                       const BookmarkNode* node) {}

  
  virtual void BookmarkNodeFaviconChanged(BookmarkModel* model,
                                          const BookmarkNode* node) = 0;

  
  
  virtual void OnWillReorderBookmarkNode(BookmarkModel* model,
                                         const BookmarkNode* node) {}

  
  
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node) = 0;

  
  
  
  
  
  
  virtual void ExtensiveBookmarkChangesBeginning(BookmarkModel* model) {}

  
  
  
  virtual void ExtensiveBookmarkChangesEnded(BookmarkModel* model) {}

  
  virtual void OnWillRemoveAllBookmarks(BookmarkModel* model) {}

  
  virtual void BookmarkAllNodesRemoved(BookmarkModel* model) = 0;

 protected:
  virtual ~BookmarkModelObserver() {}
};

#endif  
