// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_QUERY_PARSER_H_
#define CHROME_BROWSER_HISTORY_QUERY_PARSER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "chrome/browser/history/snippet.h"

class QueryNodeList;

struct QueryWord {
  
  base::string16 word;

  
  size_t position;
};

class QueryNode {
 public:
  virtual ~QueryNode() {}

  
  
  virtual int AppendToSQLiteQuery(base::string16* query) const = 0;

  
  virtual bool IsWord() const = 0;

  
  
  virtual bool Matches(const base::string16& word, bool exact) const = 0;

  
  
  
  virtual bool HasMatchIn(const std::vector<QueryWord>& words,
                          Snippet::MatchPositions* match_positions) const = 0;

  
  virtual bool HasMatchIn(const std::vector<QueryWord>& words) const = 0;

  
  virtual void AppendWords(std::vector<base::string16>* words) const = 0;
};

class QueryParser {
 public:
  QueryParser();

  
  
  
  
  
  
  
  static bool IsWordLongEnoughForPrefixSearch(const base::string16& word);

  
  
  int ParseQuery(const base::string16& query, base::string16* sqlite_query);

  
  
  
  
  void ParseQueryWords(const base::string16& query,
                       std::vector<base::string16>* words);

  
  
  
  void ParseQueryNodes(const base::string16& query,
                       std::vector<QueryNode*>* nodes);

  
  
  
  bool DoesQueryMatch(const base::string16& text,
                      const std::vector<QueryNode*>& nodes,
                      Snippet::MatchPositions* match_positions);

  
  
  bool DoesQueryMatch(const std::vector<QueryWord>& words,
                      const std::vector<QueryNode*>& nodes);

  
  void ExtractQueryWords(const base::string16& text,
                         std::vector<QueryWord>* words);

 private:
  
  
  bool ParseQueryImpl(const base::string16& query, QueryNodeList* root);

  DISALLOW_COPY_AND_ASSIGN(QueryParser);
};

#endif  
