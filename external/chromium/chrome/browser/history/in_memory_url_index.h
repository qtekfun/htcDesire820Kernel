// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_IN_MEMORY_URL_INDEX_H_
#define CHROME_BROWSER_HISTORY_IN_MEMORY_URL_INDEX_H_
#pragma once

#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "app/sql/connection.h"
#include "base/basictypes.h"
#include "base/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/string16.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/autocomplete/history_provider_util.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/in_memory_url_index_cache.pb.h"
#include "testing/gtest/include/gtest/gtest_prod.h"

class Profile;

namespace base {
class Time;
}

namespace in_memory_url_index {
class InMemoryURLIndexCacheItem;
}

namespace history {

namespace imui = in_memory_url_index;

class URLDatabase;

struct TermMatch {
  TermMatch(int term_num, size_t offset, size_t length)
      : term_num(term_num),
        offset(offset),
        length(length) {}

  int term_num;  
  size_t offset;  
  size_t length;  
};
typedef std::vector<TermMatch> TermMatches;

struct ScoredHistoryMatch : public HistoryMatch {
  ScoredHistoryMatch();  
  explicit ScoredHistoryMatch(const URLRow& url_info);
  ~ScoredHistoryMatch();

  static bool MatchScoreGreater(const ScoredHistoryMatch& m1,
                                const ScoredHistoryMatch& m2);

  
  
  int raw_score;
  TermMatches url_matches;  
  TermMatches title_matches;  
  size_t prefix_adjust;  
};
typedef std::vector<ScoredHistoryMatch> ScoredHistoryMatches;

class InMemoryURLIndex {
 public:
  
  
  
  explicit InMemoryURLIndex(const FilePath& history_dir);
  ~InMemoryURLIndex();

  
  typedef std::vector<string16> String16Vector;

  
  
  
  
  bool Init(URLDatabase* history_db, const std::string& languages);

  
  
  
  bool ReloadFromHistory(URLDatabase* history_db, bool clear_cache);

  
  
  void ShutDown();

  
  
  bool RestoreFromCacheFile();

  
  
  bool SaveToCacheFile();

  
  
  
  
  
  
  
  
  
  
  
  
  ScoredHistoryMatches HistoryItemsForTerms(const String16Vector& terms);

  
  
  void UpdateURL(URLID row_id, const URLRow& row);

  
  
  
  void DeleteURL(URLID row_id);

  
  
  
  
  
  
  
  
  
  
  
  
  static String16Vector WordVectorFromString16(const string16& uni_string,
                                               bool break_on_space);

  
  static std::vector<size_t> OffsetsFromTermMatches(const TermMatches& matches);

  
  
  static TermMatches ReplaceOffsetsInTermMatches(
      const TermMatches& matches,
      const std::vector<size_t>& offsets);

 private:
  friend class AddHistoryMatch;
  FRIEND_TEST_ALL_PREFIXES(LimitedInMemoryURLIndexTest, Initialization);
  FRIEND_TEST_ALL_PREFIXES(InMemoryURLIndexTest, CacheFilePath);
  FRIEND_TEST_ALL_PREFIXES(InMemoryURLIndexTest, CacheSaveRestore);
  FRIEND_TEST_ALL_PREFIXES(InMemoryURLIndexTest, Char16Utilities);
  FRIEND_TEST_ALL_PREFIXES(InMemoryURLIndexTest, Scoring);
  FRIEND_TEST_ALL_PREFIXES(InMemoryURLIndexTest, StaticFunctions);
  FRIEND_TEST_ALL_PREFIXES(InMemoryURLIndexTest, TitleSearch);
  FRIEND_TEST_ALL_PREFIXES(InMemoryURLIndexTest, TypedCharacterCaching);

  
  static const size_t kNoCachedResultForTerm;

  
  InMemoryURLIndex();

  
  typedef std::set<string16> String16Set;
  typedef std::set<char16> Char16Set;
  typedef std::vector<char16> Char16Vector;

  
  typedef int WordID;

  
  typedef std::map<string16, WordID> WordMap;

  
  typedef std::set<WordID> WordIDSet;  
  typedef std::map<char16, WordIDSet> CharWordIDMap;

  
  
  
  typedef URLID HistoryID;
  typedef std::set<HistoryID> HistoryIDSet;
  typedef std::map<WordID, HistoryIDSet> WordIDHistoryMap;

  
  
  
  
  
  
  
  
  
  
  struct TermCharWordSet;
  typedef std::vector<TermCharWordSet> TermCharWordSetVector;

  
  typedef std::vector<URLRow> URLRowVector;

  
  typedef std::map<HistoryID, URLRow> HistoryInfoMap;

  
  
  
  class AddHistoryMatch : public std::unary_function<HistoryID, void> {
   public:
    AddHistoryMatch(const InMemoryURLIndex& index,
                    const String16Vector& lower_terms);
    ~AddHistoryMatch();

    void operator()(const HistoryID history_id);

    ScoredHistoryMatches ScoredMatches() const { return scored_matches_; }

   private:
    const InMemoryURLIndex& index_;
    ScoredHistoryMatches scored_matches_;
    const String16Vector& lower_terms_;
  };

  
  
  void ClearPrivateData();

  
  static String16Set WordSetFromString16(const string16& uni_string);

  
  
  
  
  WordIDSet WordIDSetForTermChars(const Char16Vector& uni_chars);

  
  
  
  
  
  
  size_t CachedResultsIndexForTerm(const Char16Vector& uni_chars);

  
  
  
  
  static TermMatches MatchTermInString(const string16& term,
                                       const string16& string,
                                       int term_num);

  

  
  bool IndexRow(const URLRow& row);

  
  
  static Char16Vector Char16VectorFromString16(const string16& uni_word);

  
  
  
  
  
  
  
  
  static Char16Set Char16SetFromString16(const string16& uni_word);

  
  
  void AddWordToIndex(const string16& uni_word, HistoryID history_id);

  
  
  void UpdateWordHistory(WordID word_id, HistoryID history_id);

  
  
  void AddWordHistory(const string16& uni_word, HistoryID history_id);

  
  
  
  void ResetTermCharWordSetCache();

  
  
  HistoryIDSet HistoryIDSetFromWords(const string16& uni_string);

  
  
  HistoryIDSet HistoryIDsForTerm(const string16& uni_word);

  
  
  
  
  
  
  
  
  
  
  static ScoredHistoryMatch ScoredMatchForURL(
      const URLRow& row,
      const String16Vector& terms_vector);

  
  
  
  static int ScoreComponentForMatches(const TermMatches& matches,
                                      size_t max_length);

  
  
  static TermMatches SortAndDeoverlap(const TermMatches& matches);

  

  
  
  
  
  bool GetCacheFilePath(FilePath* file_path);

  
  void SavePrivateData(imui::InMemoryURLIndexCacheItem* cache) const;
  void SaveWordList(imui::InMemoryURLIndexCacheItem* cache) const;
  void SaveWordMap(imui::InMemoryURLIndexCacheItem* cache) const;
  void SaveCharWordMap(imui::InMemoryURLIndexCacheItem* cache) const;
  void SaveWordIDHistoryMap(imui::InMemoryURLIndexCacheItem* cache) const;
  void SaveHistoryInfoMap(imui::InMemoryURLIndexCacheItem* cache) const;

  
  
  bool RestorePrivateData(const imui::InMemoryURLIndexCacheItem& cache);
  bool RestoreWordList(const imui::InMemoryURLIndexCacheItem& cache);
  bool RestoreWordMap(const imui::InMemoryURLIndexCacheItem& cache);
  bool RestoreCharWordMap(const imui::InMemoryURLIndexCacheItem& cache);
  bool RestoreWordIDHistoryMap(const imui::InMemoryURLIndexCacheItem& cache);
  bool RestoreHistoryInfoMap(const imui::InMemoryURLIndexCacheItem& cache);

  
  
  
  FilePath history_dir_;

  
  
  
  
  base::Time last_saved_;

  
  
  
  
  
  
  
  
  String16Vector word_list_;

  int history_item_count_;
  WordMap word_map_;
  CharWordIDMap char_word_map_;
  WordIDHistoryMap word_id_history_map_;
  TermCharWordSetVector term_char_word_set_cache_;
  HistoryInfoMap history_info_map_;
  std::string languages_;

  DISALLOW_COPY_AND_ASSIGN(InMemoryURLIndex);
};

}  

#endif  
