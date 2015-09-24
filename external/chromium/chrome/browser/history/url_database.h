// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_URL_DATABASE_H_
#define CHROME_BROWSER_HISTORY_URL_DATABASE_H_
#pragma once

#include "app/sql/statement.h"
#include "base/basictypes.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/search_engines/template_url_id.h"

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

  
  
  bool GetAllTypedUrls(std::vector<history::URLRow>* urls);

  
  
  
  
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

  
  
  class IconMappingEnumerator : public URLEnumeratorBase {
   public:
    IconMappingEnumerator();

    
    bool GetNextIconMapping(IconMapping* r);

   private:
    DISALLOW_COPY_AND_ASSIGN(IconMappingEnumerator);
  };

  
  bool InitURLEnumeratorForEverything(URLEnumerator* enumerator);

  
  
  
  
  bool InitURLEnumeratorForSignificant(URLEnumerator* enumerator);

  

  

  
  
  
  
  void AutocompleteForPrefix(const string16& prefix,
                             size_t max_results,
                             bool typed_only,
                             std::vector<URLRow>* results);

  
  
  
  
  
  bool FindShortestURLFromBase(const std::string& base,
                               const std::string& url,
                               int min_visits,
                               int min_typed,
                               bool allow_base,
                               history::URLRow* info);

  

  
  bool SetKeywordSearchTermsForURL(URLID url_id,
                                   TemplateURLID keyword_id,
                                   const string16& term);

  
  
  bool GetKeywordSearchTermRow(URLID url_id, KeywordSearchTermRow* row);

  
  
  void DeleteAllSearchTermsForKeyword(TemplateURLID keyword_id);

  
  
  void GetMostRecentKeywordSearchTerms(
      TemplateURLID keyword_id,
      const string16& prefix,
      int max_count,
      std::vector<KeywordSearchTermVisit>* matches);

  

  
  
  
  bool MigrateFromVersion11ToVersion12();

  
  
  bool InitIconMappingEnumeratorForEverything(
      IconMappingEnumerator* enumerator);

 protected:
  friend class VisitDatabase;

  
  static const char kURLRowFields[];

  
  
  
  static const int kNumURLRowFields;

  
  
  
  bool DropStarredIDFromURLs();

  
  
  
  
  
  
  
  
  bool CreateURLTable(bool is_temporary);
  
  
  void CreateMainURLIndex();

  
  bool InitKeywordSearchTermsTable();

  
  void CreateKeywordSearchTermsIndices();

  
  bool DropKeywordSearchTermsTable();

  
  
  
  
  URLID AddURLInternal(const URLRow& info, bool is_temporary);

  
  
  static void FillURLRow(sql::Statement& s, URLRow* i);

  
  
  virtual sql::Connection& GetDB() = 0;

 private:
  
  
  bool has_keyword_search_terms_;

  DISALLOW_COPY_AND_ASSIGN(URLDatabase);
};

#define HISTORY_URL_ROW_FIELDS \
    " urls.id, urls.url, urls.title, urls.visit_count, urls.typed_count, " \
    "urls.last_visit_time, urls.hidden "

}  

#endif  
