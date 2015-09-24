// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_HISTORY_QUERY_PARSER_H_
#define CHROME_BROWSER_HISTORY_QUERY_PARSER_H_
#pragma once

#include <vector>

#include "base/string16.h"
#include "chrome/browser/history/snippet.h"

class QueryNodeList;

struct QueryWord {
  
  string16 word;

  
  size_t position;
};

class QueryNode {
 public:
  virtual ~QueryNode() {}

  
  
  virtual int AppendToSQLiteQuery(string16* query) const = 0;

  
  virtual bool IsWord() const = 0;

  
  
  
  virtual bool Matches(const string16& word, bool exact) const = 0;

  
  
  
  virtual bool HasMatchIn(const std::vector<QueryWord>& words,
                          Snippet::MatchPositions* match_positions) const = 0;

  
  virtual void AppendWords(std::vector<string16>* words) const = 0;
};


class QueryParser {
 public:
  QueryParser();

  
  
  
  
  
  
  
  static bool IsWordLongEnoughForPrefixSearch(const string16& word);

  
  
  int ParseQuery(const string16& query,
                 string16* sqlite_query);

  
  
  
  
  void ParseQuery(const string16& query,
                  std::vector<QueryNode*>* nodes);

  
  
  
  
  void ExtractQueryWords(const string16& query,
                         std::vector<string16>* words);

  
  
  
  bool DoesQueryMatch(const string16& text,
                      const std::vector<QueryNode*>& nodes,
                      Snippet::MatchPositions* match_positions);

 private:
  
  
  bool ParseQueryImpl(const string16& query,
                      QueryNodeList* root);

  
  void ExtractQueryWords(const string16& text,
                         std::vector<QueryWord>* words);
};

#endif  
