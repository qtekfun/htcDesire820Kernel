// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_SCORED_HISTORY_MATCH_H_
#define CHROME_BROWSER_HISTORY_SCORED_HISTORY_MATCH_H_

#include <map>
#include <set>
#include <vector>

#include "base/strings/string16.h"
#include "chrome/browser/autocomplete/history_provider_util.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/in_memory_url_index_types.h"
#include "testing/gtest/include/gtest/gtest_prod.h"

class BookmarkService;

namespace history {

class ScoredHistoryMatchTest;

class ScoredHistoryMatch : public history::HistoryMatch {
 public:
  
  
  
  static const size_t kMaxVisitsToScore;

  ScoredHistoryMatch();  

  
  
  
  
  
  
  
  
  
  
  ScoredHistoryMatch(const URLRow& row,
                     const VisitInfoVector& visits,
                     const std::string& languages,
                     const base::string16& lower_string,
                     const String16Vector& terms_vector,
                     const RowWordStarts& word_starts,
                     const base::Time now,
                     BookmarkService* bookmark_service);
  ~ScoredHistoryMatch();

  
  
  
  static bool MatchScoreGreater(const ScoredHistoryMatch& m1,
                                const ScoredHistoryMatch& m2);

  
  int raw_score() const { return raw_score_; }
  const TermMatches& url_matches() const { return url_matches_; }
  const TermMatches& title_matches() const { return title_matches_; }
  bool can_inline() const { return can_inline_; }

  
  
  
  
  
  static TermMatches FilterTermMatchesByWordStarts(
      const TermMatches& term_matches,
      const WordStarts& word_starts,
      size_t start_pos,
      size_t end_pos);

 private:
  friend class ScoredHistoryMatchTest;
  FRIEND_TEST_ALL_PREFIXES(ScoredHistoryMatchTest, ScoringBookmarks);
  FRIEND_TEST_ALL_PREFIXES(ScoredHistoryMatchTest, ScoringDiscountFrecency);
  FRIEND_TEST_ALL_PREFIXES(ScoredHistoryMatchTest, ScoringScheme);
  FRIEND_TEST_ALL_PREFIXES(ScoredHistoryMatchTest, ScoringTLD);

  
  static const int kDaysToPrecomputeRecencyScoresFor;

  
  
  static const int kMaxRawTermScore;

  
  
  
  
  
  float GetTopicalityScore(const int num_terms,
                           const base::string16& cleaned_up_url,
                           const RowWordStarts& word_starts);

  
  
  static void FillInTermScoreToTopicalityScoreArray();

  
  
  static float GetRecencyScore(int last_visit_days_ago);

  
  
  static void FillInDaysAgoToRecencyScoreArray();

  
  
  
  
  
  static float GetFrecency(const base::Time& now,
                           const bool bookmarked,
                           const VisitInfoVector& visits);

  
  
  static float GetFinalRelevancyScore(
      float topicality_score,
      float frecency_score);

  
  
  
  
  static void Init();

  
  
  int raw_score_;

  
  
  
  
  
  
  
  TermMatches url_matches_;
  
  TermMatches title_matches_;

  
  bool can_inline_;

  
  
  
  
  
  
  static float* days_ago_to_recency_score_;

  
  
  
  
  
  
  
  
  static float* raw_term_score_to_topicality_score_;

  
  static bool initialized_;

  
  
  static int bookmark_value_;

  
  
  
  
  
  
  
  
  static bool discount_frecency_when_few_visits_;

  
  static bool allow_tld_matches_;

  
  static bool allow_scheme_matches_;

  
  
  
  
  static bool also_do_hup_like_scoring_;

  
  
  
  
  
  
  static int max_assigned_score_for_non_inlineable_matches_;
};
typedef std::vector<ScoredHistoryMatch> ScoredHistoryMatches;

}  

#endif  
