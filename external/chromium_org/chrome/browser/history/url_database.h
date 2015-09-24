// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_URL_DATABASE_H_
#define CHROME_BROWSER_HISTORY_URL_DATABASE_H_

#include "base/basictypes.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/query_parser.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "sql/statement.h"

class GURL;

namespace sql {
class Connection;
}

namespace history {

class VisitDatabase;  

class URLDatabase {
 public:
  
  
  URLDatabase();

  
  
  virtual ~URLDatabase();

  
  
  
  
  
  
  
  
  
  static std::string GURLToDatabaseURL(const GURL& url);

  

  
  
  bool GetURLRow(URLID url_id, URLRow* info);

  
  
  bool GetAllTypedUrls(URLRows* urls);

  
  
  
  
  URLID GetRowForURL(const GURL& url, URLRow* info);

  
  
  
  
  
  bool UpdateURLRow(URLID url_id, const URLRow& info);

  
  
  
  
  
  URLID AddURL(const URLRow& info) {
    return AddURLInternal(info, false);
  }

  
  
  
  bool DeleteURLRow(URLID id);

  

  
  
  
  
  bool CreateTemporaryURLTable();

  
  
  
  
  URLID AddTemporaryURL(const URLRow& row) {
    return AddURLInternal(row, true);
  }

  
  
  
  
  
  virtual bool CommitTemporaryURLTable();

  

  
  class URLEnumeratorBase {
   public:
    URLEnumeratorBase();
    virtual ~URLEnumeratorBase();

   private:
    friend class URLDatabase;

    bool initialized_;
    sql::Statement statement_;

    DISALLOW_COPY_AND_ASSIGN(URLEnumeratorBase);
  };

  
  class URLEnumerator : public URLEnumeratorBase {
   public:
    URLEnumerator();

    
    bool GetNextURL(history::URLRow* r);

   private:
    DISALLOW_COPY_AND_ASSIGN(URLEnumerator);
  };

  
  bool InitURLEnumeratorForEverything(URLEnumerator* enumerator);

  
  
  
  
  bool InitURLEnumeratorForSignificant(URLEnumerator* enumerator);

  

  

  
  
  
  
  
  bool AutocompleteForPrefix(const std::string& prefix,
                             size_t max_results,
                             bool typed_only,
                             URLRows* results);

  
  
  bool IsTypedHost(const std::string& host);

  
  
  
  
  
  bool FindShortestURLFromBase(const std::string& base,
                               const std::string& url,
                               int min_visits,
                               int min_typed,
                               bool allow_base,
                               history::URLRow* info);

  

  
  
  bool GetTextMatches(const base::string16& query, URLRows* results);

  

  
  bool SetKeywordSearchTermsForURL(URLID url_id,
                                   TemplateURLID keyword_id,
                                   const base::string16& term);

  
  
  bool GetKeywordSearchTermRow(URLID url_id, KeywordSearchTermRow* row);

  
  
  bool GetKeywordSearchTermRows(const base::string16& term,
                                std::vector<KeywordSearchTermRow>* rows);

  
  
  void DeleteAllSearchTermsForKeyword(TemplateURLID keyword_id);

  
  
  void GetMostRecentKeywordSearchTerms(
      TemplateURLID keyword_id,
      const base::string16& prefix,
      int max_count,
      std::vector<KeywordSearchTermVisit>* matches);

  
  bool DeleteKeywordSearchTerm(const base::string16& term);

  
  bool DeleteKeywordSearchTermForURL(URLID url_id);

  

  
  
  
  bool MigrateFromVersion11ToVersion12();

 protected:
  friend class VisitDatabase;

  
  static const char kURLRowFields[];

  
  
  
  static const int kNumURLRowFields;

  
  
  
  bool DropStarredIDFromURLs();

  
  
  
  
  
  
  
  
  bool CreateURLTable(bool is_temporary);
  
  
  bool CreateMainURLIndex();

  
  bool InitKeywordSearchTermsTable();

  
  bool CreateKeywordSearchTermsIndices();

  
  bool DropKeywordSearchTermsTable();

  
  
  
  
  URLID AddURLInternal(const URLRow& info, bool is_temporary);

  
  
  static void FillURLRow(sql::Statement& s, URLRow* i);

  
  
  virtual sql::Connection& GetDB() = 0;

 private:
  
  
  bool has_keyword_search_terms_;

  QueryParser query_parser_;

  DISALLOW_COPY_AND_ASSIGN(URLDatabase);
};

#define HISTORY_URL_ROW_FIELDS \
    " urls.id, urls.url, urls.title, urls.visit_count, urls.typed_count, " \
    "urls.last_visit_time, urls.hidden "

}  

#endif  
