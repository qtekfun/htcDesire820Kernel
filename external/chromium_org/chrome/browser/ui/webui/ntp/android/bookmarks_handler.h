// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_NTP_ANDROID_BOOKMARKS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_NTP_ANDROID_BOOKMARKS_HANDLER_H_

#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "chrome/browser/android/bookmarks/managed_bookmarks_shim.h"
#include "chrome/browser/android/bookmarks/partner_bookmarks_shim.h"
#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "chrome/browser/favicon/favicon_service.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "content/public/browser/web_ui_message_handler.h"

class BookmarksHandler : public content::WebUIMessageHandler,
                         public BaseBookmarkModelObserver,
                         public PartnerBookmarksShim::Observer,
                         public ManagedBookmarksShim::Observer {
 public:
  BookmarksHandler();
  virtual ~BookmarksHandler();

  
  virtual void RegisterMessages() OVERRIDE;

  
  void HandleGetBookmarks(const ListValue* args);
  
  void HandleDeleteBookmark(const ListValue* args);
  
  void HandleEditBookmark(const ListValue* args);
  
  
  
  void HandleCreateHomeScreenBookmarkShortcut(const base::ListValue* args);

  
  virtual void Loaded(BookmarkModel* model, bool ids_reassigned) OVERRIDE;
  virtual void BookmarkModelChanged() OVERRIDE;
  virtual void ExtensiveBookmarkChangesBeginning(BookmarkModel* model) OVERRIDE;
  virtual void ExtensiveBookmarkChangesEnded(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int old_index,
                                   const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkAllNodesRemoved(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkNodeAdded(
      BookmarkModel* model, const BookmarkNode* parent, int index) OVERRIDE;
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node) OVERRIDE;

  
  virtual void PartnerShimChanged(PartnerBookmarksShim* shim) OVERRIDE;
  virtual void PartnerShimLoaded(PartnerBookmarksShim* shim) OVERRIDE;
  virtual void ShimBeingDeleted(PartnerBookmarksShim* shim) OVERRIDE;

  
  virtual void OnManagedBookmarksChanged() OVERRIDE;

 private:
  
  BookmarkModel* bookmark_model_;

  
  PartnerBookmarksShim* partner_bookmarks_shim_;

  
  scoped_ptr<ManagedBookmarksShim> managed_bookmarks_shim_;

  
  bool bookmark_data_requested_;

  
  bool extensive_changes_;

  
  CancelableTaskTracker cancelable_task_tracker_;

  
  bool AreModelsLoaded() const;

  
  void NotifyModelChanged(const DictionaryValue& status);

  
  std::string GetBookmarkIdForNtp(const BookmarkNode* node);

  
  
  void SetParentInBookmarksResult(const BookmarkNode* parent,
                                  DictionaryValue* result);

  
  
  void PopulateBookmark(const BookmarkNode* node, ListValue* result);

  
  
  void PopulateBookmarksInFolder(const BookmarkNode* folder,
                                 DictionaryValue* result);

  
  void QueryBookmarkFolder(const BookmarkNode* node);

  
  
  void QueryInitialBookmarks();

  
  void SendResult(const DictionaryValue& result);

  
  
  void OnShortcutFaviconDataAvailable(
      const BookmarkNode* node,
      const chrome::FaviconBitmapResult& bitmap_result);

  
  
  const BookmarkNode* GetNodeByID(const base::ListValue* args) const;

  
  const BookmarkNode* GetParentOf(const BookmarkNode* node) const;

  
  base::string16 GetTitle(const BookmarkNode* node) const;

  
  bool IsReachable(const BookmarkNode* node) const;

  
  bool IsEditable(const BookmarkNode* node) const;

  
  
  bool IsRoot(const BookmarkNode* node) const;

  DISALLOW_COPY_AND_ASSIGN(BookmarksHandler);
};

#endif  
