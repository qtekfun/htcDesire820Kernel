// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_MODEL_OBSERVER_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_MODEL_OBSERVER_H_
#pragma once

class BookmarkModel;
class BookmarkNode;

class BookmarkModelObserver {
 public:
  
  virtual void Loaded(BookmarkModel* model) = 0;

  
  virtual void BookmarkModelBeingDeleted(BookmarkModel* model) {}

  
  virtual void BookmarkNodeMoved(BookmarkModel* model,
                                 const BookmarkNode* old_parent,
                                 int old_index,
                                 const BookmarkNode* new_parent,
                                 int new_index) = 0;

  
  virtual void BookmarkNodeAdded(BookmarkModel* model,
                                 const BookmarkNode* parent,
                                 int index) = 0;

  
  
  
  
  
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int old_index,
                                   const BookmarkNode* node) = 0;

  
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node) = 0;

  
  virtual void BookmarkNodeFaviconLoaded(BookmarkModel* model,
                                         const BookmarkNode* node) = 0;

  
  
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node) = 0;

  
  
  
  
  
  virtual void BookmarkImportBeginning(BookmarkModel* model) {}

  
  
  virtual void BookmarkImportEnding(BookmarkModel* model) {}

 protected:
  virtual ~BookmarkModelObserver() {}
};

#endif  
