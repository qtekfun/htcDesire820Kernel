// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_TEXT_DATABASE_H_
#define CHROME_BROWSER_HISTORY_TEXT_DATABASE_H_
#pragma once

#include <set>
#include <vector>

#include "app/sql/connection.h"
#include "app/sql/meta_table.h"
#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/string16.h"
#include "chrome/browser/history/history_types.h"
#include "googleurl/src/gurl.h"

namespace history {

class TextDatabase {
 public:
  typedef int DBIdent;

  typedef std::set<GURL> URLSet;

  
  struct Match {
    Match();
    ~Match();

    
    GURL url;

    
    
    
    string16 title;

    
    base::Time time;

    
    
    Snippet::MatchPositions title_match_positions;

    
    Snippet snippet;
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  TextDatabase(const FilePath& path,
               DBIdent id,
               bool allow_create);
  ~TextDatabase();

  
  
  
  
  bool Init();

  
  
  
  void BeginTransaction();
  void CommitTransaction();

  
  
  const FilePath& file_name() const { return file_name_; }

  
  
  
  static const FilePath::CharType* file_base();

  
  
  static DBIdent FileNameToID(const FilePath& file_path);

  

  
  
  bool AddPageData(base::Time time,
                   const std::string& url,
                   const std::string& title,
                   const std::string& contents);

  
  void DeletePageData(base::Time time, const std::string& url);

  
  
  
  
  
  
  void Optimize();

  

  
  
  
  
  
  
  
  
  
  
  
  
  void GetTextMatches(const std::string& query,
                      const QueryOptions& options,
                      std::vector<Match>* results,
                      URLSet* unique_urls,
                      base::Time* first_time_searched);

  
  
  static FilePath IDToFileName(DBIdent id);

 private:
  
  bool CreateTables();

  
  sql::Connection db_;

  const FilePath path_;
  const DBIdent ident_;
  const bool allow_create_;

  
  FilePath file_name_;

  sql::MetaTable meta_table_;

  DISALLOW_COPY_AND_ASSIGN(TextDatabase);
};

}  

#endif  
