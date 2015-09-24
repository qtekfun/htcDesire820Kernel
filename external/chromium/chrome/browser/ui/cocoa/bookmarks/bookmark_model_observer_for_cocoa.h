// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_MODEL_OBSERVER_FOR_COCOA_H
#define CHROME_BROWSER_UI_COCOA_BOOKMARKS_BOOKMARK_MODEL_OBSERVER_FOR_COCOA_H
#pragma once

#import <Cocoa/Cocoa.h>

#include "base/basictypes.h"
#include "base/memory/scoped_nsobject.h"
#include "chrome/browser/bookmarks/bookmark_model.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"

class BookmarkModelObserverForCocoa : public BookmarkModelObserver {
 public:
  
  
  
  
  
  
  
  
  
  BookmarkModelObserverForCocoa(const BookmarkNode* node,
                                BookmarkModel* model,
                                NSObject* object,
                                SEL selector);
  virtual ~BookmarkModelObserverForCocoa();

  virtual void BookmarkModelBeingDeleted(BookmarkModel* model);
  virtual void BookmarkNodeMoved(BookmarkModel* model,
                                 const BookmarkNode* old_parent,
                                 int old_index,
                                 const BookmarkNode* new_parent,
                                 int new_index);
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int old_index,
                                   const BookmarkNode* node);
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node);
  virtual void BookmarkImportBeginning(BookmarkModel* model);

  
  
  virtual void Loaded(BookmarkModel* model) {
  }
  virtual void BookmarkNodeAdded(BookmarkModel* model,
                                 const BookmarkNode* parent,
                                 int index) {
  }
  virtual void BookmarkNodeFaviconLoaded(BookmarkModel* model,
                                         const BookmarkNode* node) {
  }
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node) {
  }

  virtual void BookmarkImportEnding(BookmarkModel* model) {
  }

 private:
  const BookmarkNode* node_;  
  BookmarkModel* model_;  
  NSObject* object_; 
  SEL selector_;

  void Notify();

  DISALLOW_COPY_AND_ASSIGN(BookmarkModelObserverForCocoa);
};

#endif  
