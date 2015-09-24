// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ANDROID_BOOKMARKS_MANAGED_BOOKMARKS_SHIM_H_
#define CHROME_BROWSER_ANDROID_BOOKMARKS_MANAGED_BOOKMARKS_SHIM_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/pref_change_registrar.h"

class BookmarkNode;
class PrefService;

class ManagedBookmarksShim {
 public:
  class Observer {
   public:
    virtual ~Observer() {}
    virtual void OnManagedBookmarksChanged() = 0;
  };

  
  
  
  explicit ManagedBookmarksShim(PrefService* prefs);
  ~ManagedBookmarksShim();

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  bool HasManagedBookmarks() const;
  bool IsManagedBookmark(const BookmarkNode* node) const;
  const BookmarkNode* GetManagedBookmarksRoot() const;
  const BookmarkNode* GetNodeByID(int64 id) const;
  const BookmarkNode* GetParentOf(const BookmarkNode* node) const;

 private:
  void Reload();

  PrefService* prefs_;
  PrefChangeRegistrar registrar_;
  scoped_ptr<BookmarkNode> root_;
  ObserverList<Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(ManagedBookmarksShim);
};

#endif  
