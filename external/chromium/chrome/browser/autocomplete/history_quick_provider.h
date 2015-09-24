// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_HISTORY_QUICK_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_HISTORY_QUICK_PROVIDER_H_
#pragma once

#include <string>

#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/autocomplete/history_provider.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/history/in_memory_url_index.h"

class Profile;
class TermMatches;

namespace history {
class HistoryBackend;
}  

class HistoryQuickProvider : public HistoryProvider {
 public:
  HistoryQuickProvider(ACProviderListener* listener, Profile* profile);

  ~HistoryQuickProvider();

  
  
  virtual void Start(const AutocompleteInput& input,
                     bool minimal_changes) OVERRIDE;

  virtual void DeleteMatch(const AutocompleteMatch& match) OVERRIDE;

  
  void DoAutocomplete();

 private:
  friend class HistoryQuickProviderTest;
  FRIEND_TEST_ALL_PREFIXES(HistoryQuickProviderTest, Spans);

  AutocompleteMatch QuickMatchToACMatch(
      const history::ScoredHistoryMatch& history_match,
      size_t match_number,
      bool prevent_inline_autocomplete,
      int* next_dont_inline_score);

  
  
  
  
  static int CalculateRelevance(int raw_score,
                                AutocompleteInput::Type input_type,
                                MatchType match_type,
                                size_t match_number);

  
  history::InMemoryURLIndex* GetIndex();

  
  
  static ACMatchClassifications SpansFromTermMatch(
      const history::TermMatches& matches,
      size_t text_length);

  
  void SetIndexForTesting(history::InMemoryURLIndex* index);
  AutocompleteInput autocomplete_input_;
  std::string languages_;

  
  scoped_ptr<history::InMemoryURLIndex> index_for_testing_;
};

#endif  
