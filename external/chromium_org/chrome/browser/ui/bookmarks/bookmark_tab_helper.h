// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_TAB_HELPER_H_
#define CHROME_BROWSER_UI_BOOKMARKS_BOOKMARK_TAB_HELPER_H_

#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"

struct BookmarkNodeData;
class BookmarkTabHelperDelegate;

namespace content {
class WebContents;
}

class BookmarkTabHelper
    : public BaseBookmarkModelObserver,
      public content::WebContentsObserver,
      public content::WebContentsUserData<BookmarkTabHelper> {
 public:
  
  class BookmarkDrag {
   public:
    virtual void OnDragEnter(const BookmarkNodeData& data) = 0;
    virtual void OnDragOver(const BookmarkNodeData& data) = 0;
    virtual void OnDragLeave(const BookmarkNodeData& data) = 0;
    virtual void OnDrop(const BookmarkNodeData& data) = 0;

   protected:
    virtual ~BookmarkDrag() {}
  };

  virtual ~BookmarkTabHelper();

  void set_delegate(BookmarkTabHelperDelegate* delegate) {
    delegate_ = delegate;
  }

  
  
  
  void set_bookmark_drag_delegate(BookmarkDrag* bookmark_drag) {
    bookmark_drag_ = bookmark_drag;
  }
  BookmarkDrag* bookmark_drag_delegate() { return bookmark_drag_; }

  bool is_starred() const { return is_starred_; }

  
  bool ShouldShowBookmarkBar() const;

 private:
  friend class content::WebContentsUserData<BookmarkTabHelper>;

  explicit BookmarkTabHelper(content::WebContents* web_contents);

  
  
  void UpdateStarredStateForCurrentURL();

  
  virtual void BookmarkModelChanged() OVERRIDE;
  virtual void Loaded(BookmarkModel* model, bool ids_reassigned) OVERRIDE;
  virtual void BookmarkNodeAdded(BookmarkModel* model,
                                 const BookmarkNode* parent,
                                 int index) OVERRIDE;
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int old_index,
                                   const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkAllNodesRemoved(BookmarkModel* model) OVERRIDE;

  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node) OVERRIDE;

  
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;

  
  bool is_starred_;

  BookmarkModel* bookmark_model_;

  
  BookmarkTabHelperDelegate* delegate_;

  
  
  BookmarkDrag* bookmark_drag_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkTabHelper);
};

#endif  
