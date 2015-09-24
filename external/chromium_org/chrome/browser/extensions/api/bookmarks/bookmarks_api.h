// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_BOOKMARKS_BOOKMARKS_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_BOOKMARKS_BOOKMARKS_API_H_

#include <list>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "extensions/browser/event_router.h"
#include "ui/shell_dialogs/select_file_dialog.h"

class Profile;

namespace base {
class FilePath;
class ListValue;
}

namespace extensions {

class BookmarkEventRouter : public BookmarkModelObserver {
 public:
  BookmarkEventRouter(Profile* profile, BookmarkModel* model);
  virtual ~BookmarkEventRouter();

  
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
  virtual void BookmarkNodeRemoved(BookmarkModel* model,
                                   const BookmarkNode* parent,
                                   int old_index,
                                   const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkAllNodesRemoved(BookmarkModel* model) OVERRIDE;
  virtual void BookmarkNodeChanged(BookmarkModel* model,
                                   const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeFaviconChanged(BookmarkModel* model,
                                          const BookmarkNode* node) OVERRIDE;
  virtual void BookmarkNodeChildrenReordered(BookmarkModel* model,
                                             const BookmarkNode* node) OVERRIDE;
  virtual void ExtensiveBookmarkChangesBeginning(BookmarkModel* model) OVERRIDE;
  virtual void ExtensiveBookmarkChangesEnded(BookmarkModel* model) OVERRIDE;

 private:
  
  void DispatchEvent(const std::string& event_name,
                     scoped_ptr<base::ListValue> event_args);

  Profile* profile_;
  BookmarkModel* model_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkEventRouter);
};

class BookmarksAPI : public ProfileKeyedAPI,
                     public EventRouter::Observer {
 public:
  explicit BookmarksAPI(Profile* profile);
  virtual ~BookmarksAPI();

  
  virtual void Shutdown() OVERRIDE;

  
  static ProfileKeyedAPIFactory<BookmarksAPI>* GetFactoryInstance();

  
  virtual void OnListenerAdded(
      const EventListenerInfo& details) OVERRIDE;

 private:
  friend class ProfileKeyedAPIFactory<BookmarksAPI>;

  Profile* profile_;

  
  static const char* service_name() {
    return "BookmarksAPI";
  }
  static const bool kServiceIsNULLWhileTesting = true;

  
  scoped_ptr<BookmarkEventRouter> bookmark_event_router_;
};

class BookmarksFunction : public ChromeAsyncExtensionFunction,
                          public BaseBookmarkModelObserver {
 public:
  
  virtual void Run() OVERRIDE;

 protected:
  virtual ~BookmarksFunction() {}

  
  
  
  bool GetBookmarkIdAsInt64(const std::string& id_string, int64* id);

  
  
  
  const BookmarkNode* GetBookmarkNodeFromId(const std::string& id_string);

  
  
  bool EditBookmarksEnabled();

 private:
  
  virtual void BookmarkModelChanged() OVERRIDE;
  virtual void Loaded(BookmarkModel* model, bool ids_reassigned) OVERRIDE;
};

class BookmarksGetFunction : public BookmarksFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bookmarks.get", BOOKMARKS_GET)

 protected:
  virtual ~BookmarksGetFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class BookmarksGetChildrenFunction : public BookmarksFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bookmarks.getChildren", BOOKMARKS_GETCHILDREN)

 protected:
  virtual ~BookmarksGetChildrenFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class BookmarksGetRecentFunction : public BookmarksFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bookmarks.getRecent", BOOKMARKS_GETRECENT)

 protected:
  virtual ~BookmarksGetRecentFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class BookmarksGetTreeFunction : public BookmarksFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bookmarks.getTree", BOOKMARKS_GETTREE)

 protected:
  virtual ~BookmarksGetTreeFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class BookmarksGetSubTreeFunction : public BookmarksFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bookmarks.getSubTree", BOOKMARKS_GETSUBTREE)

 protected:
  virtual ~BookmarksGetSubTreeFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class BookmarksSearchFunction : public BookmarksFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bookmarks.search", BOOKMARKS_SEARCH)

 protected:
  virtual ~BookmarksSearchFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class BookmarksRemoveFunction : public BookmarksFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bookmarks.remove", BOOKMARKS_REMOVE)

  
  
  static bool ExtractIds(const base::ListValue* args,
                         std::list<int64>* ids,
                         bool* invalid_id);
  
  virtual void GetQuotaLimitHeuristics(
      QuotaLimitHeuristics* heuristics) const OVERRIDE;

 protected:
  virtual ~BookmarksRemoveFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class BookmarksRemoveTreeFunction : public BookmarksRemoveFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bookmarks.removeTree", BOOKMARKS_REMOVETREE)

 protected:
  virtual ~BookmarksRemoveTreeFunction() {}
};

class BookmarksCreateFunction : public BookmarksFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bookmarks.create", BOOKMARKS_CREATE)

  
  virtual void GetQuotaLimitHeuristics(
      QuotaLimitHeuristics* heuristics) const OVERRIDE;

 protected:
  virtual ~BookmarksCreateFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class BookmarksMoveFunction : public BookmarksFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bookmarks.move", BOOKMARKS_MOVE)

  static bool ExtractIds(const base::ListValue* args,
                         std::list<int64>* ids,
                         bool* invalid_id);

  
  virtual void GetQuotaLimitHeuristics(
      QuotaLimitHeuristics* heuristics) const OVERRIDE;

 protected:
  virtual ~BookmarksMoveFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class BookmarksUpdateFunction : public BookmarksFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bookmarks.update", BOOKMARKS_UPDATE)

  static bool ExtractIds(const base::ListValue* args,
                         std::list<int64>* ids,
                         bool* invalid_id);

  
  virtual void GetQuotaLimitHeuristics(
      QuotaLimitHeuristics* heuristics) const OVERRIDE;

 protected:
  virtual ~BookmarksUpdateFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class BookmarksIOFunction : public BookmarksFunction,
                            public ui::SelectFileDialog::Listener {
 public:
  BookmarksIOFunction();

  virtual void FileSelected(const base::FilePath& path, int index, void* params) = 0;

  
  virtual void MultiFilesSelected(const std::vector<base::FilePath>& files,
                                  void* params) OVERRIDE;
  virtual void FileSelectionCanceled(void* params) OVERRIDE;

  void SelectFile(ui::SelectFileDialog::Type type);

 protected:
  virtual ~BookmarksIOFunction();

 private:
  void ShowSelectFileDialog(
      ui::SelectFileDialog::Type type,
      const base::FilePath& default_path);

 protected:
  scoped_refptr<ui::SelectFileDialog> select_file_dialog_;
};

class BookmarksImportFunction : public BookmarksIOFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bookmarks.import", BOOKMARKS_IMPORT)

  
  virtual void FileSelected(const base::FilePath& path,
                            int index,
                            void* params) OVERRIDE;

 private:
  virtual ~BookmarksImportFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class BookmarksExportFunction : public BookmarksIOFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("bookmarks.export", BOOKMARKS_EXPORT)

  
  virtual void FileSelected(const base::FilePath& path,
                            int index,
                            void* params) OVERRIDE;

 private:
  virtual ~BookmarksExportFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

}  

#endif  
