// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_MODEL_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_MODEL_H_

#include <map>
#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/strings/string16.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "chrome/browser/bookmarks/bookmark_service.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "ui/base/models/tree_node_model.h"
#include "ui/gfx/image/image.h"
#include "url/gurl.h"

class BookmarkExpandedStateTracker;
class BookmarkIndex;
class BookmarkLoadDetails;
class BookmarkModel;
class BookmarkModelObserver;
class BookmarkStorage;
struct BookmarkTitleMatch;
class Profile;

namespace base {
class SequencedTaskRunner;
}

namespace chrome {
struct FaviconImageResult;
}


class BookmarkNode : public ui::TreeNode<BookmarkNode> {
 public:
  enum Type {
    URL,
    FOLDER,
    BOOKMARK_BAR,
    OTHER_NODE,
    MOBILE
  };

  enum FaviconState {
    INVALID_FAVICON,
    LOADING_FAVICON,
    LOADED_FAVICON,
  };

  typedef std::map<std::string, std::string> MetaInfoMap;

  static const int64 kInvalidSyncTransactionVersion;

  
  explicit BookmarkNode(const GURL& url);
  
  BookmarkNode(int64 id, const GURL& url);

  virtual ~BookmarkNode();

  
  
  
  virtual void SetTitle(const base::string16& title) OVERRIDE;

  
  
  
  int64 id() const { return id_; }
  void set_id(int64 id) { id_ = id; }

  const GURL& url() const { return url_; }
  void set_url(const GURL& url) { url_ = url; }

  
  
  const GURL& icon_url() const { return icon_url_; }

  Type type() const { return type_; }
  void set_type(Type type) { type_ = type; }

  
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

  bool is_favicon_loaded() const { return favicon_state_ == LOADED_FAVICON; }

  
  
  
  
  
  virtual bool IsVisible() const;

  
  
  
  bool GetMetaInfo(const std::string& key, std::string* value) const;
  bool SetMetaInfo(const std::string& key, const std::string& value);
  bool DeleteMetaInfo(const std::string& key);
  void SetMetaInfoMap(const MetaInfoMap& meta_info_map);
  
  const MetaInfoMap* GetMetaInfoMap() const;

  void set_sync_transaction_version(int64 sync_transaction_version) {
    sync_transaction_version_ = sync_transaction_version;
  }
  int64 sync_transaction_version() const {
    return sync_transaction_version_;
  }

  
  

 private:
  friend class BookmarkModel;

  
  void Initialize(int64 id);

  
  void InvalidateFavicon();

  
  void set_icon_url(const GURL& icon_url) {
    icon_url_ = icon_url;
  }

  const gfx::Image& favicon() const { return favicon_; }
  void set_favicon(const gfx::Image& icon) { favicon_ = icon; }

  FaviconState favicon_state() const { return favicon_state_; }
  void set_favicon_state(FaviconState state) { favicon_state_ = state; }

  CancelableTaskTracker::TaskId favicon_load_task_id() const {
    return favicon_load_task_id_;
  }
  void set_favicon_load_task_id(CancelableTaskTracker::TaskId id) {
    favicon_load_task_id_ = id;
  }

  
  int64 id_;

  
  
  GURL url_;

  
  Type type_;

  
  base::Time date_added_;

  
  base::Time date_folder_modified_;

  
  gfx::Image favicon_;

  
  GURL icon_url_;

  
  FaviconState favicon_state_;

  
  
  CancelableTaskTracker::TaskId favicon_load_task_id_;

  
  scoped_ptr<MetaInfoMap> meta_info_map_;

  
  int64 sync_transaction_version_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkNode);
};


class BookmarkPermanentNode : public BookmarkNode {
 public:
  explicit BookmarkPermanentNode(int64 id);
  virtual ~BookmarkPermanentNode();

  
  void set_visible(bool value) { visible_ = value; }

  
  virtual bool IsVisible() const OVERRIDE;

 private:
  bool visible_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkPermanentNode);
};


class BookmarkModel : public content::NotificationObserver,
                      public BookmarkService,
                      public BrowserContextKeyedService {
 public:
  explicit BookmarkModel(Profile* profile);
  virtual ~BookmarkModel();

  
  virtual void Shutdown() OVERRIDE;

  
  
  
  void Load(const scoped_refptr<base::SequencedTaskRunner>& task_runner);

  
  bool loaded() const { return loaded_; }

  
  
  const BookmarkNode* root_node() { return &root_; }

  
  const BookmarkNode* bookmark_bar_node() { return bookmark_bar_node_; }

  
  const BookmarkNode* other_node() { return other_node_; }

  
  const BookmarkNode* mobile_node() { return mobile_node_; }

  bool is_root_node(const BookmarkNode* node) const { return node == &root_; }

  
  
  bool is_permanent_node(const BookmarkNode* node) const {
    return node == &root_ ||
           node == bookmark_bar_node_ ||
           node == other_node_ ||
           node == mobile_node_;
  }

  
  
  const BookmarkNode* GetParentForNewNodes();

  void AddObserver(BookmarkModelObserver* observer);
  void RemoveObserver(BookmarkModelObserver* observer);

  
  
  
  void BeginExtensiveChanges();
  void EndExtensiveChanges();

  
  
  
  
  bool IsDoingExtensiveChanges() const { return extensive_changes_ > 0; }

  
  
  void Remove(const BookmarkNode* parent, int index);

  
  
  
  void RemoveAll();

  
  void Move(const BookmarkNode* node,
            const BookmarkNode* new_parent,
            int index);

  
  void Copy(const BookmarkNode* node,
            const BookmarkNode* new_parent,
            int index);

  
  
  const gfx::Image& GetFavicon(const BookmarkNode* node);

  
  void SetTitle(const BookmarkNode* node, const base::string16& title);

  
  void SetURL(const BookmarkNode* node, const GURL& url);

  
  void SetDateAdded(const BookmarkNode* node, base::Time date_added);

  
  void GetNodesByURL(const GURL& url, std::vector<const BookmarkNode*>* nodes);

  
  
  const BookmarkNode* GetMostRecentlyAddedNodeForURL(const GURL& url);

  
  
  bool HasBookmarks();

  
  
  
  virtual bool IsBookmarked(const GURL& url) OVERRIDE;

  
  
  
  virtual void GetBookmarks(
      std::vector<BookmarkService::URLAndTitle>* urls) OVERRIDE;

  
  
  virtual void BlockTillLoaded() OVERRIDE;

  
  const BookmarkNode* GetNodeByID(int64 id) const;

  
  const BookmarkNode* AddFolder(const BookmarkNode* parent,
                                int index,
                                const base::string16& title);

  
  const BookmarkNode* AddURL(const BookmarkNode* parent,
                             int index,
                             const base::string16& title,
                             const GURL& url);

  
  const BookmarkNode* AddURLWithCreationTime(const BookmarkNode* parent,
                                             int index,
                                             const base::string16& title,
                                             const GURL& url,
                                             const base::Time& creation_time);

  
  
  void SortChildren(const BookmarkNode* parent);

  
  
  
  
  void ReorderChildren(const BookmarkNode* parent,
                       const std::vector<const BookmarkNode*>& ordered_nodes);

  
  void SetDateFolderModified(const BookmarkNode* node, const base::Time time);

  
  
  
  void ResetDateFolderModified(const BookmarkNode* node);

  void GetBookmarksWithTitlesMatching(
      const base::string16& text,
      size_t max_count,
      std::vector<BookmarkTitleMatch>* matches);

  
  
  
  void ClearStore();

  
  int64 next_node_id() const { return next_node_id_; }

  
  
  BookmarkExpandedStateTracker* expanded_state_tracker() {
    return expanded_state_tracker_.get();
  }

  
  void SetPermanentNodeVisible(BookmarkNode::Type type, bool value);

  
  void SetNodeMetaInfo(const BookmarkNode* node,
                       const std::string& key,
                       const std::string& value);
  void SetNodeMetaInfoMap(const BookmarkNode* node,
                          const BookmarkNode::MetaInfoMap& meta_info_map);
  void DeleteNodeMetaInfo(const BookmarkNode* node,
                          const std::string& key);

  
  void SetNodeSyncTransactionVersion(const BookmarkNode* node,
                                     int64 sync_transaction_version);

 private:
  friend class BookmarkCodecTest;
  friend class BookmarkModelTest;
  friend class BookmarkStorage;

  
  class NodeURLComparator {
   public:
    bool operator()(const BookmarkNode* n1, const BookmarkNode* n2) const {
      return n1->url() < n2->url();
    }
  };

  
  
  bool IsBookmarkedNoLock(const GURL& url);

  
  
  
  
  void RemoveNode(BookmarkNode* node, std::set<GURL>* removed_urls);

  
  
  void DoneLoading(BookmarkLoadDetails* details);

  
  void PopulateNodesByURL(BookmarkNode* node);

  
  
  
  
  void RemoveNodeAndGetRemovedUrls(BookmarkNode* node,
                                   std::set<GURL>* removed_urls);

  
  
  void RemoveAndDeleteNode(BookmarkNode* delete_me);

  
  void NotifyHistoryAboutRemovedBookmarks(
      const std::set<GURL>& removed_bookmark_urls) const;

  
  
  BookmarkNode* AddNode(BookmarkNode* parent,
                        int index,
                        BookmarkNode* node);

  
  const BookmarkNode* GetNodeByID(const BookmarkNode* node, int64 id) const;

  
  bool IsValidIndex(const BookmarkNode* parent, int index, bool allow_end);

  
  
  BookmarkPermanentNode* CreatePermanentNode(BookmarkNode::Type type);

  
  
  void OnFaviconDataAvailable(BookmarkNode* node,
                              const chrome::FaviconImageResult& image_result);

  
  
  void LoadFavicon(BookmarkNode* node);

  
  void FaviconLoaded(const BookmarkNode* node);

  
  void CancelPendingFaviconLoadRequests(BookmarkNode* node);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  int64 generate_next_node_id();

  
  
  
  void set_next_node_id(int64 id) { next_node_id_ = id; }

  
  
  BookmarkLoadDetails* CreateLoadDetails();

  content::NotificationRegistrar registrar_;

  Profile* profile_;

  
  bool loaded_;

  
  
  BookmarkNode root_;

  BookmarkPermanentNode* bookmark_bar_node_;
  BookmarkPermanentNode* other_node_;
  BookmarkPermanentNode* mobile_node_;

  
  int64 next_node_id_;

  
  ObserverList<BookmarkModelObserver> observers_;

  
  
  
  
  typedef std::multiset<BookmarkNode*, NodeURLComparator> NodesOrderedByURLSet;
  NodesOrderedByURLSet nodes_ordered_by_url_set_;
  base::Lock url_lock_;

  
  CancelableTaskTracker cancelable_task_tracker_;

  
  scoped_refptr<BookmarkStorage> store_;

  scoped_ptr<BookmarkIndex> index_;

  base::WaitableEvent loaded_signal_;

  
  int extensive_changes_;

  scoped_ptr<BookmarkExpandedStateTracker> expanded_state_tracker_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkModel);
};

#endif  
