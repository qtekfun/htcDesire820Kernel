// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_TAG_MODEL_OBSERVER_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_TAG_MODEL_OBSERVER_H_

class BookmarkTagModel;
class BookmarkNode;

class BookmarkTagModelObserver {
 public:
  
  virtual void Loaded(BookmarkTagModel* model) = 0;

  
  virtual void BookmarkTagModelBeingDeleted(BookmarkTagModel* model) {}

  
  virtual void BookmarkNodeAdded(BookmarkTagModel* model,
                                 const BookmarkNode* bookmark) = 0;

  
  
  virtual void OnWillRemoveBookmarks(BookmarkTagModel* model,
                                     const BookmarkNode* bookmark) {}

  
  virtual void BookmarkNodeRemoved(BookmarkTagModel* model,
                                   const BookmarkNode* bookmark) = 0;

  
  virtual void OnWillChangeBookmarkNode(BookmarkTagModel* model,
                                        const BookmarkNode* bookmark) {}

  
  virtual void BookmarkNodeChanged(BookmarkTagModel* model,
                                   const BookmarkNode* bookmark) = 0;

  
  virtual void OnWillChangeBookmarkTags(BookmarkTagModel* model,
                                        const BookmarkNode* bookmark) {}

  
  virtual void BookmarkTagsChanged(BookmarkTagModel* model,
                                   const BookmarkNode* bookmark) = 0;

  
  virtual void BookmarkNodeFaviconChanged(BookmarkTagModel* model,
                                          const BookmarkNode* node) = 0;

  
  
  
  
  
  
  virtual void ExtensiveBookmarkChangesBeginning(BookmarkTagModel* model) {}

  
  
  
  virtual void ExtensiveBookmarkChangesEnded(BookmarkTagModel* model) {}

  
  virtual void OnWillRemoveAllBookmarks(BookmarkTagModel* model) {}

  
  virtual void BookmarkAllNodesRemoved(BookmarkTagModel* model) = 0;

 protected:
  virtual ~BookmarkTagModelObserver() {}
};

#endif  
