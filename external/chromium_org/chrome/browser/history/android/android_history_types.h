// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_ANDROID_ANDROID_HISTORY_TYPES_H_
#define CHROME_BROWSER_HISTORY_ANDROID_ANDROID_HISTORY_TYPES_H_

#include <map>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "sql/statement.h"

namespace sql {
class Statement;
}

namespace history {

typedef int64 AndroidURLID;
typedef int64 SearchTermID;

class HistoryAndBookmarkRow {
 public:
  enum ColumnID {
    ID,
    URL,
    TITLE,
    CREATED,
    LAST_VISIT_TIME,
    VISIT_COUNT,
    FAVICON,
    BOOKMARK,
    RAW_URL,
    PARENT_ID,
    URL_ID,
    COLUMN_END 
  };

  HistoryAndBookmarkRow();
  virtual ~HistoryAndBookmarkRow();

  
  static std::string GetAndroidName(ColumnID id);

  static ColumnID GetColumnID(const std::string& name);

  
  void set_url(const GURL& url) {
    set_value_explicitly(URL);
    url_ = url;
  }
  const GURL& url() const {
    return url_;
  }

  
  void set_raw_url(const std::string& raw_url) {
    set_value_explicitly(RAW_URL);
    raw_url_ = raw_url;
  }
  const std::string& raw_url() const {
    return raw_url_;
  }

  
  void set_title(const base::string16& title) {
    set_value_explicitly(TITLE);
    title_ = title;
  }
  const base::string16& title() const {
    return title_;
  }

  
  void set_created(const base::Time created) {
    set_value_explicitly(CREATED);
    created_ = created;
  }
  const base::Time& created() const {
    return created_;
  }

  
  void set_last_visit_time(const base::Time last_visit_time) {
    set_value_explicitly(LAST_VISIT_TIME);
    last_visit_time_ = last_visit_time;
  }
  const base::Time& last_visit_time() const {
    return last_visit_time_;
  }

  
  void set_visit_count(int visit_count) {
    set_value_explicitly(VISIT_COUNT);
    visit_count_ = visit_count;
  }
  int visit_count() const {
    return visit_count_;
  }

  
  void set_is_bookmark(bool is_bookmark) {
    set_value_explicitly(BOOKMARK);
    is_bookmark_ = is_bookmark;
  }
  bool is_bookmark() const {
    return is_bookmark_;
  }

  
  void set_favicon(const scoped_refptr<base::RefCountedMemory>& data) {
    set_value_explicitly(FAVICON);
    favicon_ = data;
  }
  const scoped_refptr<base::RefCountedMemory>& favicon() const {
    return favicon_;
  }

  bool favicon_valid() const {
    return favicon_.get() && favicon_->size();
  }

  
  void set_id(AndroidURLID id) {
    set_value_explicitly(ID);
    id_ = id;
  }
  AndroidURLID id() const {
    return id_;
  }

  
  void set_parent_id(int64 parent_id) {
    set_value_explicitly(PARENT_ID);
    parent_id_ = parent_id;
  }
  const int64 parent_id() const {
    return parent_id_;
  }

  
  void set_url_id(URLID url_id) {
    set_value_explicitly(URL_ID);
    url_id_ = url_id;
  }
  URLID url_id() const {
    return url_id_;
  }

  
  bool is_value_set_explicitly(ColumnID id) const {
    return values_set_.find(id) != values_set_.end();
  }

 private:
  void set_value_explicitly(ColumnID id) {
    values_set_.insert(id);
  }

  AndroidURLID id_;
  GURL url_;
  std::string raw_url_;
  base::string16 title_;
  base::Time created_;
  base::Time last_visit_time_;
  scoped_refptr<base::RefCountedMemory> favicon_;
  int visit_count_;
  bool is_bookmark_;
  int64 parent_id_;
  URLID url_id_;

  
  std::set<ColumnID> values_set_;

  
};

class SearchRow {
 public:
  enum ColumnID {
    ID,
    SEARCH_TERM,
    SEARCH_TIME,
    URL,
    TEMPLATE_URL,
    COLUMN_END
  };

  SearchRow();
  virtual ~SearchRow();

  
  static std::string GetAndroidName(ColumnID id);

  static ColumnID GetColumnID(const std::string& name);

  SearchTermID id() const {
    return id_;
  }
  void set_id(SearchTermID id) {
    set_value_explicitly(SearchRow::ID);
    id_ = id;
  }

  const base::string16& search_term() const {
    return search_term_;
  }
  void set_search_term(const base::string16& search_term) {
    set_value_explicitly(SearchRow::SEARCH_TERM);
    search_term_ = search_term;
  }

  const base::Time search_time() const {
    return search_time_;
  }
  void set_search_time(const base::Time& time) {
    set_value_explicitly(SearchRow::SEARCH_TIME);
    search_time_ = time;
  }

  const GURL& url() const {
    return url_;
  }
  void set_url(const GURL& url) {
    set_value_explicitly(SearchRow::URL);
    url_ = url;
  }

  TemplateURLID template_url_id() const {
    return template_url_id_;
  }
  void set_template_url_id(TemplateURLID template_url_id) {
    set_value_explicitly(SearchRow::TEMPLATE_URL);
    template_url_id_ = template_url_id;
  }

 
  bool is_value_set_explicitly(ColumnID id) const {
    return values_set_.find(id) != values_set_.end();
  }

 private:
  void set_value_explicitly(ColumnID id) {
    values_set_.insert(id);
  }

  SearchTermID id_;
  base::string16 search_term_;
  base::Time search_time_;
  GURL url_;
  TemplateURLID template_url_id_;

  
  std::set<ColumnID> values_set_;

  
};

struct AndroidURLRow {
  AndroidURLRow();
  ~AndroidURLRow();

  
  AndroidURLID id;
  
  URLID url_id;
  
  std::string raw_url;
};

struct SearchTermRow {
  SearchTermRow();
  ~SearchTermRow();

  
  SearchTermID id;
  
  base::string16 term;
  
  base::Time last_visit_time;
};

class AndroidStatement {
 public:
  AndroidStatement(sql::Statement* statement, int favicon_index);
  ~AndroidStatement();

  sql::Statement* statement() {
    return statement_.get();
  }

  
  
  int favicon_index() const {
    return favicon_index_;
  }

 private:
  scoped_ptr<sql::Statement> statement_;
  int favicon_index_;

  DISALLOW_COPY_AND_ASSIGN(AndroidStatement);
};

}  

#endif  
