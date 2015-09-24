// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_TAG_MODEL_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_TAG_MODEL_H_

#include "chrome/browser/bookmarks/bookmark_model.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"

class BookmarkTagModelObserver;

typedef base::string16 BookmarkTag;

class BookmarkTagModel : public BookmarkModelObserver {
 public:
  explicit BookmarkTagModel(BookmarkModel* bookmark_model);
  virtual ~BookmarkTagModel();

  
  bool loaded() const { return loaded_; }

  
  void AddObserver(BookmarkTagModelObserver* observer);
  void RemoveObserver(BookmarkTagModelObserver* observer);

  
  
  class ExtensiveChanges {
   public:
    friend class BookmarkTagModel;
    explicit ExtensiveChanges(BookmarkTagModel* model) : model_(model) {
      model_->BeginExtensiveChanges();
    }
   private:
    ~ExtensiveChanges() {
      model_->EndExtensiveChanges();
    }

    BookmarkTagModel* model_;
  };

  
  
  
  
  bool IsDoingExtensiveChanges() const;

  
  void Remove(const BookmarkNode* bookmark);

  
  
  void RemoveAll();

  
  
  const gfx::Image& GetFavicon(const BookmarkNode* bookmark);

  
  void SetTitle(const BookmarkNode* bookmark, const base::string16& title);

  
  void SetURL(const BookmarkNode* bookmark, const GURL& url);

  
  void SetDateAdded(const BookmarkNode* bookmark, base::Time date_added);

  
  
  const BookmarkNode* GetMostRecentlyAddedBookmarkForURL(const GURL& url);

  
  const BookmarkNode* AddURL(const base::string16& title,
                             const GURL& url,
                             const std::set<BookmarkTag>& tags);

  
  void AddTagsToBookmark(const std::set<BookmarkTag>& tags,
                         const BookmarkNode* bookmark);

  
  void AddTagsToBookmarks(const std::set<BookmarkTag>& tags,
                          const std::set<const BookmarkNode*>& bookmarks);

  
  
  void RemoveTagsFromBookmark(const std::set<BookmarkTag>& tags,
                              const BookmarkNode* bookmark);

  
  void RemoveTagsFromBookmarks(const std::set<BookmarkTag>& tags,
                               const std::set<const BookmarkNode*>& bookmarks);

  
  std::set<BookmarkTag> GetTagsForBookmark(const BookmarkNode* bookmark);

  
  std::set<const BookmarkNode*> BookmarksForTags(
      const std::set<BookmarkTag>& tags);

  
  std::set<const BookmarkNode*> BookmarksForTag(const BookmarkTag& tag);

  
  
  
  
  std::vector<BookmarkTag> TagsRelatedToTag(const BookmarkTag& tag);

  
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
  virtual void OnWillRemoveBookmarks(BookmarkModel* model,
                                     const BookmarkNode* parent,
                                     int old_index,
                                     const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int old_index,
                                   const BookmarkNode* node) OVERRIDE;
  virtual void OnWillChangeBookmarkNode(BookmarkModel* model,
                                        const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node) OVERRIDE;
  virtual void OnWillChangeBookmarkMetaInfo(BookmarkModel* model,
                                            const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkMetaInfoChanged(BookmarkModel* model,
                                       const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeFaviconChanged(BookmarkModel* model,
                                          const BookmarkNode* node) OVERRIDE;
  virtual void OnWillReorderBookmarkNode(BookmarkModel* model,
                                         const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node) OVERRIDE;
  virtual void ExtensiveBookmarkChangesBeginning(BookmarkModel* model) OVERRIDE;
  virtual void ExtensiveBookmarkChangesEnded(BookmarkModel* model) OVERRIDE;
  virtual void OnWillRemoveAllBookmarks(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkAllNodesRemoved(BookmarkModel* model) OVERRIDE;

 private:
  
  
  
  void BeginExtensiveChanges();
  void EndExtensiveChanges();

  
  
  void SetTagsOnBookmark(const std::set<BookmarkTag>& tags,
                             const BookmarkNode* bookmark);

  
  
  
  void Load();

  
  
  void ReloadDescendants(const BookmarkNode* folder);

  
  void RemoveBookmark(const BookmarkNode* bookmark);

  
  void LoadBookmark(const BookmarkNode* bookmark);

  
  BookmarkModel* bookmark_model_;

  
  bool loaded_;

  
  ObserverList<BookmarkTagModelObserver> observers_;

  
  std::map<const BookmarkTag, std::set<const BookmarkNode*> > tag_to_bookmarks_;
  std::map<const BookmarkNode*, std::set<BookmarkTag> > bookmark_to_tags_;

  
  
  bool inhibit_change_notifications_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkTagModel);
};

#endif  
