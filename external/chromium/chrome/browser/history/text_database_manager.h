// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_TEXT_DATABASE_MANAGER_H_
#define CHROME_BROWSER_HISTORY_TEXT_DATABASE_MANAGER_H_
#pragma once

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/string16.h"
#include "base/task.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/text_database.h"
#include "chrome/browser/history/query_parser.h"
#include "chrome/browser/history/url_database.h"
#include "content/common/mru_cache.h"

namespace history {

class HistoryPublisher;
class VisitDatabase;

class TextDatabaseManager {
 public:
  
  
  
  
  
  
  
  
  
  class ChangeSet {
   public:
    ChangeSet();
    ~ChangeSet();

   private:
    friend class TextDatabaseManager;

    typedef std::set<TextDatabase::DBIdent> DBSet;

    void Add(TextDatabase::DBIdent id) { changed_databases_.insert(id); }

    DBSet changed_databases_;
  };

  
  
  
  
  
  
  
  
  explicit TextDatabaseManager(const FilePath& dir,
                               URLDatabase* url_database,
                               VisitDatabase* visit_database);
  ~TextDatabaseManager();

  
  
  bool Init(const HistoryPublisher* history_publisher);

  
  const FilePath& GetDir() { return dir_; }

  
  
  
  void BeginTransaction();
  void CommitTransaction();

  
  
  
  
  
  
  
  
  
  
  void AddPageURL(const GURL& url, URLID url_id, VisitID visit_id,
                  base::Time visit_time);
  void AddPageTitle(const GURL& url, const string16& title);
  void AddPageContents(const GURL& url, const string16& body);

  
  
  
  
  bool AddPageData(const GURL& url,
                   URLID url_id,
                   VisitID visit_id,
                   base::Time visit_time,
                   const string16& title,
                   const string16& body);

  
  
  
  void DeletePageData(base::Time time, const GURL& url,
                      ChangeSet* change_set);

  
  
  
  
  
  
  
  
  
  void DeleteFromUncommitted(const std::set<GURL>& restrict_urls,
                             base::Time begin, base::Time end);

  
  
  
  void DeleteAll();

  
  
  
  void OptimizeChangedDatabases(const ChangeSet& change_set);

  
  
  
  
  
  
  
  void GetTextMatches(const string16& query,
                      const QueryOptions& options,
                      std::vector<TextDatabase::Match>* results,
                      base::Time* first_time_searched);

 private:
  
  FRIEND_TEST_ALL_PREFIXES(TextDatabaseManagerTest, InsertPartial);
  FRIEND_TEST_ALL_PREFIXES(TextDatabaseManagerTest, PartialComplete);
  FRIEND_TEST_ALL_PREFIXES(ExpireHistoryTest, DeleteURLAndFavicon);
  FRIEND_TEST_ALL_PREFIXES(ExpireHistoryTest, FlushRecentURLsUnstarred);
  FRIEND_TEST_ALL_PREFIXES(ExpireHistoryTest,
                           FlushRecentURLsUnstarredRestricted);

  
  
  class PageInfo {
   public:
    PageInfo(URLID url_id, VisitID visit_id, base::Time visit_time);
    ~PageInfo();

    
    URLID url_id() const { return url_id_; }
    VisitID visit_id() const { return visit_id_; }
    base::Time visit_time() const { return visit_time_; }
    const string16& title() const { return title_; }
    const string16& body() const { return body_; }

    
    void set_title(const string16& ttl);
    void set_body(const string16& bdy);

    
    
    
    bool has_title() const { return !title_.empty(); }
    bool has_body() { return !body_.empty(); }

    
    
    
    bool Expired(base::TimeTicks now) const;

   private:
    URLID url_id_;
    VisitID visit_id_;

    
    
    base::Time visit_time_;

    
    
    base::TimeTicks added_time_;

    
    string16 title_;
    string16 body_;
  };

  
  static TextDatabase::DBIdent TimeToID(base::Time time);
  static base::Time IDToTime(TextDatabase::DBIdent id);

  
  
  
  
  
  
  
  
  
  
  TextDatabase* GetDB(TextDatabase::DBIdent id, bool for_writing);
  TextDatabase* GetDBForTime(base::Time time, bool for_writing);

  
  
  
  void InitDBList();

  
  void ScheduleFlushOldChanges();

  
  
  void FlushOldChanges();

  
  
  
  void FlushOldChangesForTime(base::TimeTicks now);

  
  const FilePath dir_;

  
  URLDatabase* url_database_;
  VisitDatabase* visit_database_;

  
  
  
  
  
  
  
  
  typedef MRUCache<GURL, PageInfo> RecentChangeList;
  RecentChangeList recent_changes_;

  
  
  
  
  int transaction_nesting_;

  
  
  typedef OwningMRUCache<TextDatabase::DBIdent, TextDatabase*> DBCache;
  DBCache db_cache_;

  
  
  
  
  
  
  
  
  
  typedef std::set<TextDatabase::DBIdent> DBIdentSet;
  DBIdentSet present_databases_;
  bool present_databases_loaded_;  

  
  
  DBIdentSet open_transactions_;

  QueryParser query_parser_;

  
  ScopedRunnableMethodFactory<TextDatabaseManager> factory_;

  
  
  
  
  const HistoryPublisher* history_publisher_;

  DISALLOW_COPY_AND_ASSIGN(TextDatabaseManager);
};

}  

#endif  
