// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BOOKMARKS_BOOKMARK_INDEX_H_
#define CHROME_BROWSER_BOOKMARKS_BOOKMARK_INDEX_H_

#include <map>
#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"

class BookmarkNode;
struct BookmarkTitleMatch;
class QueryNode;
class QueryParser;

namespace content {
class BrowserContext;
}

namespace history {
class URLDatabase;
}

class BookmarkIndex {
 public:
  explicit BookmarkIndex(content::BrowserContext* browser_context);
  ~BookmarkIndex();

  
  void Add(const BookmarkNode* node);

  
  void Remove(const BookmarkNode* node);

  
  void GetBookmarksWithTitlesMatching(
      const base::string16& query,
      size_t max_count,
      std::vector<BookmarkTitleMatch>* results);

 private:
  typedef std::set<const BookmarkNode*> NodeSet;
  typedef std::map<base::string16, NodeSet> Index;

  struct Match;
  typedef std::vector<Match> Matches;

  
  
  typedef std::pair<const BookmarkNode*, int> NodeTypedCountPair;
  typedef std::vector<NodeTypedCountPair> NodeTypedCountPairs;

  
  
  void SortMatches(const Matches& matches,
                   NodeTypedCountPairs* node_typed_counts) const;

  
  
  
  
  void ExtractBookmarkNodePairs(history::URLDatabase* url_db,
                                const Match& match,
                                NodeTypedCountPairs* node_typed_counts) const;

  
  
  static bool NodeTypedCountPairSortFunc(const NodeTypedCountPair& a,
                                         const NodeTypedCountPair& b) {
      return a.second > b.second;
  }

  
  void AddMatchToResults(const BookmarkNode* node,
                         QueryParser* parser,
                         const std::vector<QueryNode*>& query_nodes,
                         std::vector<BookmarkTitleMatch>* results);

  
  
  
  bool GetBookmarksWithTitleMatchingTerm(const base::string16& term,
                                         bool first_term,
                                         Matches* matches);

  
  
  
  
  
  void CombineMatchesInPlace(const Index::const_iterator& index_i,
                             Matches* matches);

  
  
  
  
  
  
  
  
  
  void CombineMatches(const Index::const_iterator& index_i,
                      const Matches& current_matches,
                      Matches* result);

  
  std::vector<base::string16> ExtractQueryWords(const base::string16& query);

  
  void RegisterNode(const base::string16& term, const BookmarkNode* node);

  
  void UnregisterNode(const base::string16& term, const BookmarkNode* node);

  Index index_;

  content::BrowserContext* browser_context_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkIndex);
};

#endif  
