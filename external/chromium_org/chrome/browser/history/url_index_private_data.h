// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_URL_INDEX_PRIVATE_DATA_H_
#define CHROME_BROWSER_HISTORY_URL_INDEX_PRIVATE_DATA_H_

#include <set>
#include <string>

#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/history_service.h"
#include "chrome/browser/history/in_memory_url_index_cache.pb.h"
#include "chrome/browser/history/in_memory_url_index_types.h"
#include "chrome/browser/history/scored_history_match.h"
#include "content/public/browser/notification_details.h"

class BookmarkService;
class HistoryQuickProviderTest;

namespace in_memory_url_index {
class InMemoryURLIndexCacheItem;
}

namespace history {

namespace imui = in_memory_url_index;

class HistoryDatabase;
class InMemoryURLIndex;
class RefCountedBool;

static const int kCurrentCacheFileVersion = 4;

class URLIndexPrivateData
    : public base::RefCountedThreadSafe<URLIndexPrivateData> {
 public:
  URLIndexPrivateData();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  ScoredHistoryMatches HistoryItemsForTerms(base::string16 term_string,
                                            size_t cursor_position,
                                            const std::string& languages,
                                            BookmarkService* bookmark_service);

  
  
  
  
  
  
  
  
  
  bool UpdateURL(HistoryService* history_service,
                 const URLRow& row,
                 const std::string& languages,
                 const std::set<std::string>& scheme_whitelist);

  
  
  
  void UpdateRecentVisits(URLID url_id,
                          const VisitVector& recent_visits);

  
  
  
  
  void ScheduleUpdateRecentVisits(HistoryService* history_service,
                                  URLID url_id);

  
  
  
  
  bool DeleteURL(const GURL& url);

  
  
  
  
  
  static scoped_refptr<URLIndexPrivateData> RestoreFromFile(
      const base::FilePath& path,
      const std::string& languages);

  
  
  
  
  
  static scoped_refptr<URLIndexPrivateData> RebuildFromHistory(
      HistoryDatabase* history_db,
      const std::string& languages,
      const std::set<std::string>& scheme_whitelist);

  
  static bool WritePrivateDataToCacheFileTask(
      scoped_refptr<URLIndexPrivateData> private_data,
      const base::FilePath& file_path);

  
  
  void CancelPendingUpdates();

  
  scoped_refptr<URLIndexPrivateData> Duplicate() const;

  
  bool Empty() const;

  
  
  void Clear();

 private:
  friend class base::RefCountedThreadSafe<URLIndexPrivateData>;
  ~URLIndexPrivateData();

  friend class AddHistoryMatch;
  friend class ::HistoryQuickProviderTest;
  friend class InMemoryURLIndexTest;
  FRIEND_TEST_ALL_PREFIXES(InMemoryURLIndexTest, CacheSaveRestore);
  FRIEND_TEST_ALL_PREFIXES(InMemoryURLIndexTest, HugeResultSet);
  FRIEND_TEST_ALL_PREFIXES(InMemoryURLIndexTest, ReadVisitsFromHistory);
  FRIEND_TEST_ALL_PREFIXES(InMemoryURLIndexTest, RebuildFromHistoryIfCacheOld);
  FRIEND_TEST_ALL_PREFIXES(InMemoryURLIndexTest, Scoring);
  FRIEND_TEST_ALL_PREFIXES(InMemoryURLIndexTest, TitleSearch);
  FRIEND_TEST_ALL_PREFIXES(InMemoryURLIndexTest, TypedCharacterCaching);
  FRIEND_TEST_ALL_PREFIXES(InMemoryURLIndexTest, WhitelistedURLs);
  FRIEND_TEST_ALL_PREFIXES(LimitedInMemoryURLIndexTest, Initialization);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  struct SearchTermCacheItem {
    SearchTermCacheItem(const WordIDSet& word_id_set,
                        const HistoryIDSet& history_id_set);
    
    SearchTermCacheItem();

    ~SearchTermCacheItem();

    WordIDSet word_id_set_;
    HistoryIDSet history_id_set_;
    bool used_;  
  };
  typedef std::map<base::string16, SearchTermCacheItem> SearchTermCacheMap;

  
  
  class AddHistoryMatch : public std::unary_function<HistoryID, void> {
   public:
    AddHistoryMatch(const URLIndexPrivateData& private_data,
                    const std::string& languages,
                    BookmarkService* bookmark_service,
                    const base::string16& lower_string,
                    const String16Vector& lower_terms,
                    const base::Time now);
    ~AddHistoryMatch();

    void operator()(const HistoryID history_id);

    ScoredHistoryMatches ScoredMatches() const { return scored_matches_; }

   private:
    const URLIndexPrivateData& private_data_;
    const std::string& languages_;
    BookmarkService* bookmark_service_;
    ScoredHistoryMatches scored_matches_;
    const base::string16& lower_string_;
    const String16Vector& lower_terms_;
    const base::Time now_;
  };

  
  
  class HistoryItemFactorGreater
      : public std::binary_function<HistoryID, HistoryID, void> {
   public:
    explicit HistoryItemFactorGreater(const HistoryInfoMap& history_info_map);
    ~HistoryItemFactorGreater();

    bool operator()(const HistoryID h1, const HistoryID h2);

   private:
    const history::HistoryInfoMap& history_info_map_;
  };

  

  
  
  HistoryIDSet HistoryIDSetFromWords(const String16Vector& unsorted_words);

  
  
  HistoryIDSet HistoryIDsForTerm(const base::string16& term);

  
  WordIDSet WordIDSetForTermChars(const Char16Set& term_chars);

  
  
  
  
  
  
  
  
  
  
  bool IndexRow(HistoryDatabase* history_db,
                HistoryService* history_service,
                const URLRow& row,
                const std::string& languages,
                const std::set<std::string>& scheme_whitelist);

  
  
  
  
  void AddRowWordsToIndex(const URLRow& row,
                          RowWordStarts* word_starts,
                          const std::string& languages);

  
  
  void AddWordToIndex(const base::string16& uni_word, HistoryID history_id);

  
  
  void AddWordHistory(const base::string16& uni_word, HistoryID history_id);

  
  
  void UpdateWordHistory(WordID word_id, HistoryID history_id);

  
  
  void AddToHistoryIDWordMap(HistoryID history_id, WordID word_id);

  
  void RemoveRowFromIndex(const URLRow& row);

  
  void RemoveRowWordsFromIndex(const URLRow& row);

  
  void ResetSearchTermCache();

  
  
  bool SaveToFile(const base::FilePath& file_path);

  
  void SavePrivateData(imui::InMemoryURLIndexCacheItem* cache) const;
  void SaveWordList(imui::InMemoryURLIndexCacheItem* cache) const;
  void SaveWordMap(imui::InMemoryURLIndexCacheItem* cache) const;
  void SaveCharWordMap(imui::InMemoryURLIndexCacheItem* cache) const;
  void SaveWordIDHistoryMap(imui::InMemoryURLIndexCacheItem* cache) const;
  void SaveHistoryInfoMap(imui::InMemoryURLIndexCacheItem* cache) const;
  void SaveWordStartsMap(imui::InMemoryURLIndexCacheItem* cache) const;

  
  
  
  bool RestorePrivateData(const imui::InMemoryURLIndexCacheItem& cache,
                          const std::string& languages);
  bool RestoreWordList(const imui::InMemoryURLIndexCacheItem& cache);
  bool RestoreWordMap(const imui::InMemoryURLIndexCacheItem& cache);
  bool RestoreCharWordMap(const imui::InMemoryURLIndexCacheItem& cache);
  bool RestoreWordIDHistoryMap(const imui::InMemoryURLIndexCacheItem& cache);
  bool RestoreHistoryInfoMap(const imui::InMemoryURLIndexCacheItem& cache);
  bool RestoreWordStartsMap(const imui::InMemoryURLIndexCacheItem& cache,
                            const std::string& languages);

  
  static bool URLSchemeIsWhitelisted(const GURL& gurl,
                                     const std::set<std::string>& whitelist);

  
  SearchTermCacheMap search_term_cache_;

  
  CancelableRequestConsumer recent_visits_consumer_;

  

  
  
  
  int restored_cache_version_;

  
  base::Time last_time_rebuilt_from_history_;

  
  
  
  String16Vector word_list_;

  
  
  
  
  
  
  
  
  WordIDSet available_words_;

  
  
  WordMap word_map_;

  
  
  CharWordIDMap char_word_map_;

  
  
  WordIDHistoryMap word_id_history_map_;

  
  
  
  HistoryIDWordMap history_id_word_map_;

  
  
  HistoryInfoMap history_info_map_;

  
  
  WordStartsMap word_starts_map_;

  

  
  
  
  int saved_cache_version_;

  
  
  size_t pre_filter_item_count_;    
  size_t post_filter_item_count_;   
  size_t post_scoring_item_count_;  
};

}  

#endif  
