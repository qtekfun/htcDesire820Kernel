// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ANDROID_BOOKMARKS_PARTNER_BOOKMARKS_SHIM_H_
#define CHROME_BROWSER_ANDROID_BOOKMARKS_PARTNER_BOOKMARKS_SHIM_H_

#include "base/android/jni_helper.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "base/supports_user_data.h"
#include "chrome/browser/bookmarks/bookmark_model.h"
#include "url/gurl.h"

class PrefService;

namespace content {
class WebContents;
}

namespace user_prefs {
class PrefRegistrySyncable;
}

class PartnerBookmarksShim : public base::SupportsUserData::Data {
 public:
  
  static PartnerBookmarksShim* BuildForBrowserContext(
      content::BrowserContext* browser_context);

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  
  bool IsLoaded() const;

  
  bool HasPartnerBookmarks() const;

  
  
  bool IsReachable(const BookmarkNode* node) const;

  
  
  void RemoveBookmark(const BookmarkNode* node);

  
  void RenameBookmark(const BookmarkNode* node, const base::string16& title);

  
  class Observer {
   public:
    
    virtual void PartnerShimChanged(PartnerBookmarksShim*) {}
    
    virtual void PartnerShimLoaded(PartnerBookmarksShim*) {}
    
    virtual void ShimBeingDeleted(PartnerBookmarksShim*) {}
   protected:
    virtual ~Observer() {}
  };

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  const BookmarkNode* GetNodeByID(int64 id) const;
  base::string16 GetTitle(const BookmarkNode* node) const;

  bool IsPartnerBookmark(const BookmarkNode* node) const;
  const BookmarkNode* GetPartnerBookmarksRoot() const;

  
  
  void SetPartnerBookmarksRoot(BookmarkNode* root_node);

  
  
  
  class NodeRenamingMapKey {
   public:
    NodeRenamingMapKey(const GURL& url, const base::string16& provider_title);
    ~NodeRenamingMapKey();
    const GURL& url() const { return url_; }
    const base::string16& provider_title() const { return provider_title_; }
    friend bool operator<(const NodeRenamingMapKey& a,
                          const NodeRenamingMapKey& b);
   private:
    GURL url_;
    base::string16 provider_title_;
  };
  typedef std::map<NodeRenamingMapKey, base::string16> NodeRenamingMap;

  
  static void ClearInBrowserContextForTesting(
      content::BrowserContext* browser_context);

  
  static void ClearPartnerModelForTesting();

 private:
  explicit PartnerBookmarksShim(PrefService* prefs);
  virtual ~PartnerBookmarksShim();

  const BookmarkNode* GetNodeByID(const BookmarkNode* parent, int64 id) const;
  void ReloadNodeMapping();
  void SaveNodeMapping();

  scoped_ptr<BookmarkNode> partner_bookmarks_root_;
  PrefService* prefs_;
  NodeRenamingMap node_rename_remove_map_;

  
  ObserverList<Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(PartnerBookmarksShim);
};

#endif  
