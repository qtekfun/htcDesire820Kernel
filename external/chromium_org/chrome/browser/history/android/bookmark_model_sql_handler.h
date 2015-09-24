// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_ANDROID_BOOKMARK_MODEL_SQL_HANDLER_H_
#define CHROME_BROWSER_HISTORY_ANDROID_BOOKMARK_MODEL_SQL_HANDLER_H_

#include "chrome/browser/history/android/sql_handler.h"

class BookmarkModel;
class Profile;

namespace history {

class HistoryDatabase;

class BookmarkModelSQLHandler : public SQLHandler {
 public:
  explicit BookmarkModelSQLHandler(URLDatabase* url_database);

  virtual ~BookmarkModelSQLHandler();

  
  virtual bool Update(const HistoryAndBookmarkRow& row,
                      const TableIDRows& ids_set) OVERRIDE;
  virtual bool Delete(const TableIDRows& ids_set) OVERRIDE;
  virtual bool Insert(HistoryAndBookmarkRow* row) OVERRIDE;

 private:
  
  
  
  class Task : public base::RefCountedThreadSafe<Task> {
   public:
    
    
    
    
    
    
    Task();

    
    void AddBookmarkToMobileFolder(const GURL& url,
                                   const base::string16& title);

    
    void AddBookmark(const GURL& url,
                     const base::string16& title,
                     int64 parent_id);

    
    void RemoveBookmark(const GURL& url);

    
    void UpdateBookmarkTitle(const GURL& url,
                             const base::string16& title);

   private:
    friend class base::RefCountedThreadSafe<Task>;
    ~Task();

    
    BookmarkModel* GetBookmarkModel();

    DISALLOW_COPY_AND_ASSIGN(Task);
  };

  URLDatabase* url_database_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkModelSQLHandler);
};

}  

#endif  
