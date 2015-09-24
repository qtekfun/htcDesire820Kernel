// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_MODEL_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_MODEL_H_
#pragma once

#include "build/build_config.h"

#include <set>
#include <vector>

#include "base/observer_list.h"
#include "base/string16.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "chrome/browser/bookmarks/bookmark_model_observer.h"
#include "chrome/browser/bookmarks/bookmark_service.h"
#include "chrome/browser/favicon_service.h"
#include "chrome/browser/history/history.h"
#include "chrome/browser/history/history_types.h"
#include "content/browser/cancelable_request.h"
#include "content/common/notification_registrar.h"
#include "googleurl/src/gurl.h"
#include "testing/gtest/include/gtest/gtest_prod.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "ui/base/models/tree_node_model.h"

class BookmarkIndex;
class BookmarkLoadDetails;
class BookmarkModel;
class BookmarkStorage;
class Profile;

namespace bookmark_utils {
struct TitleMatch;
}


class BookmarkNode : public ui::TreeNode<BookmarkNode> {
  friend class BookmarkModel;

 public:
  enum Type {
    URL,
    FOLDER,
    BOOKMARK_BAR,
    OTHER_NODE
  };
  
  explicit BookmarkNode(const GURL& url);
  
  BookmarkNode(int64 id, const GURL& url);
  virtual ~BookmarkNode();

  
  const GURL& GetURL() const { return url_; }
  
  void SetURL(const GURL& url) { url_ = url; }

  
  
  
  int64 id() const { return id_; }
  
  void set_id(int64 id) { id_ = id; }

  
  BookmarkNode::Type type() const { return type_; }
  void set_type(BookmarkNode::Type type) { type_ = type; }

  
  const base::Time& date_added() const { return date_added_; }
  
  void set_date_added(const base::Time& date) { date_added_ = date; }

  
  
  const base::Time& date_folder_modified() const {
    return date_folder_modified_;
  }
  
  void set_date_folder_modified(const base::Time& date) {
    date_folder_modified_ = date;
  }

  
  
  bool is_folder() const { return type_ != URL; }

  
  bool is_url() const { return type_ == URL; }

  
  
  
  
  const SkBitmap& favicon() const { return favicon_; }
  void set_favicon(const SkBitmap& icon) { favicon_ = icon; }

  
  

  bool is_favicon_loaded() const { return loaded_favicon_; }
  void set_favicon_loaded(bool value) { loaded_favicon_ = value; }

  HistoryService::Handle favicon_load_handle() const {
    return favicon_load_handle_;
  }
  void set_favicon_load_handle(HistoryService::Handle handle) {
    favicon_load_handle_ = handle;
  }

  
  void InvalidateFavicon();

  
  
  void Reset(const history::StarredEntry& entry);

  
  

 private:
  
  void Initialize(int64 id);

  
  int64 id_;

  
  bool loaded_favicon_;

  
  SkBitmap favicon_;

  
  
  HistoryService::Handle favicon_load_handle_;

  
  
  GURL url_;

  
  BookmarkNode::Type type_;

  
  base::Time date_added_;

  
  base::Time date_folder_modified_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkNode);
};



class BookmarkModel : public NotificationObserver, public BookmarkService {
  friend class BookmarkCodecTest;
  friend class BookmarkModelTest;
  friend class BookmarkStorage;

 public:
  explicit BookmarkModel(Profile* profile);
  virtual ~BookmarkModel();

  
  
  void Load();

  
  
  const BookmarkNode* root_node() { return &root_; }

  
  const BookmarkNode* GetBookmarkBarNode() { return bookmark_bar_node_; }

  
  const BookmarkNode* other_node() { return other_node_; }

  
  
  const BookmarkNode* GetParentForNewNodes();

  void AddObserver(BookmarkModelObserver* observer) {
    observers_.AddObserver(observer);
  }

  void RemoveObserver(BookmarkModelObserver* observer) {
    observers_.RemoveObserver(observer);
  }

  
  
  void BeginImportMode();
  void EndImportMode();

  
  
  void Remove(const BookmarkNode* parent, int index);

  
  void Move(const BookmarkNode* node,
            const BookmarkNode* new_parent,
            int index);

  
  void Copy(const BookmarkNode* node,
            const BookmarkNode* new_parent,
            int index);

  
  
  const SkBitmap& GetFavicon(const BookmarkNode* node);

  
  void SetTitle(const BookmarkNode* node, const string16& title);

  
  void SetURL(const BookmarkNode* node, const GURL& url);

  
  virtual bool IsLoaded();

  
  void GetNodesByURL(const GURL& url, std::vector<const BookmarkNode*>* nodes);

  
  
  const BookmarkNode* GetMostRecentlyAddedNodeForURL(const GURL& url);

  
  virtual void GetBookmarks(std::vector<GURL>* urls);

  
  
  bool HasBookmarks();

  
  
  virtual bool IsBookmarked(const GURL& url);

  
  
  virtual void BlockTillLoaded();

  
  
  const BookmarkNode* GetNodeByID(int64 id);

  
  const BookmarkNode* AddFolder(const BookmarkNode* parent,
                                int index,
                                const string16& title);

  
  const BookmarkNode* AddURL(const BookmarkNode* parent,
                             int index,
                             const string16& title,
                             const GURL& url);

  
  const BookmarkNode* AddURLWithCreationTime(const BookmarkNode* parent,
                                             int index,
                                             const string16& title,
                                             const GURL& url,
                                             const base::Time& creation_time);

  
  
  void SortChildren(const BookmarkNode* parent);

  
  
  
  void SetURLStarred(const GURL& url,
                     const string16& title,
                     bool is_starred);

  
  void SetDateFolderModified(const BookmarkNode* parent, const base::Time time);

  
  
  
  void ResetDateFolderModified(const BookmarkNode* node);

  void GetBookmarksWithTitlesMatching(
      const string16& text,
      size_t max_count,
      std::vector<bookmark_utils::TitleMatch>* matches);

  Profile* profile() const { return profile_; }

  bool is_root(const BookmarkNode* node) const { return node == &root_; }
  bool is_bookmark_bar_node(const BookmarkNode* node) const {
    return node == bookmark_bar_node_;
  }
  bool is_other_bookmarks_node(const BookmarkNode* node) const {
    return node == other_node_;
  }
  
  
  bool is_permanent_node(const BookmarkNode* node) const {
    return is_root(node) ||
           is_bookmark_bar_node(node) ||
           is_other_bookmarks_node(node);
  }

  
  
  
  void ClearStore();

  
  bool file_changed() const { return file_changed_; }

  
  int64 next_node_id() const { return next_node_id_; }

 private:
  
  class NodeURLComparator {
   public:
    bool operator()(const BookmarkNode* n1, const BookmarkNode* n2) const {
      return n1->GetURL() < n2->GetURL();
    }
  };

  
  
  bool IsBookmarkedNoLock(const GURL& url);

  
  void FaviconLoaded(const BookmarkNode* node);

  
  
  
  
  void RemoveNode(BookmarkNode* node, std::set<GURL>* removed_urls);

  
  
  void DoneLoading(BookmarkLoadDetails* details);

  
  void PopulateNodesByURL(BookmarkNode* node);

  
  
  void RemoveAndDeleteNode(BookmarkNode* delete_me);

  
  
  
  BookmarkNode* AddNode(BookmarkNode* parent,
                        int index,
                        BookmarkNode* node,
                        bool was_bookmarked);

  
  const BookmarkNode* GetNodeByID(const BookmarkNode* node, int64 id);

  
  bool IsValidIndex(const BookmarkNode* parent, int index, bool allow_end);

  
  
  BookmarkNode* CreateBookmarkNode();
  BookmarkNode* CreateOtherBookmarksNode();

  
  
  BookmarkNode* CreateRootNodeFromStarredEntry(
      const history::StarredEntry& entry);

  
  
  void OnFaviconDataAvailable(FaviconService::Handle handle,
                              history::FaviconData favicon);

  
  
  void LoadFavicon(BookmarkNode* node);

  
  void CancelPendingFaviconLoadRequests(BookmarkNode* node);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  int64 generate_next_node_id();

  
  
  
  void set_next_node_id(int64 id) { next_node_id_ = id; }

  
  void SetFileChanged();

  
  
  BookmarkLoadDetails* CreateLoadDetails();

  NotificationRegistrar registrar_;

  Profile* profile_;

  
  bool loaded_;

  
  
  bool file_changed_;

  
  
  BookmarkNode root_;

  BookmarkNode* bookmark_bar_node_;
  BookmarkNode* other_node_;

  
  int64 next_node_id_;

  
  ObserverList<BookmarkModelObserver> observers_;

  
  
  
  
  typedef std::multiset<BookmarkNode*, NodeURLComparator> NodesOrderedByURLSet;
  NodesOrderedByURLSet nodes_ordered_by_url_set_;
  base::Lock url_lock_;

  
  CancelableRequestConsumerTSimple<BookmarkNode*> load_consumer_;

  
  scoped_refptr<BookmarkStorage> store_;

  scoped_ptr<BookmarkIndex> index_;

  base::WaitableEvent loaded_signal_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkModel);
};

#endif  
