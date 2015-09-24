// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_CONTROLLER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_CONTROLLER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/autocomplete/autocomplete_input.h"
#include "chrome/browser/autocomplete/autocomplete_provider.h"
#include "chrome/browser/autocomplete/autocomplete_provider_listener.h"
#include "chrome/browser/autocomplete/autocomplete_result.h"

class AutocompleteControllerDelegate;
class HistoryURLProvider;
class KeywordProvider;
class Profile;
class SearchProvider;
class ZeroSuggestProvider;

class AutocompleteController : public AutocompleteProviderListener {
 public:
  
  static const int kNoItemSelected;

  
  
  
  AutocompleteController(Profile* profile,
                         AutocompleteControllerDelegate* delegate,
                         int provider_types);
  ~AutocompleteController();

  
  
  
  
  
  
  
  
  
  
  
  
  void Start(const AutocompleteInput& input);

  
  
  
  
  
  void Stop(bool clear_result);

  
  
  
  void StartZeroSuggest(
      const GURL& url,
      AutocompleteInput::PageClassification page_classification,
      const base::string16& permanent_text);

  
  void StopZeroSuggest();

  
  
  
  void DeleteMatch(const AutocompleteMatch& match);

  
  
  void ExpireCopiedEntries();

  
  virtual void OnProviderUpdate(bool updated_matches) OVERRIDE;

  
  
  
  
  
  
  void AddProvidersInfo(ProvidersInfo* provider_info) const;

  
  
  
  void ResetSession();

  
  
  
  
  void UpdateMatchDestinationURL(base::TimeDelta query_formulation_time,
                                 AutocompleteMatch* match) const;

  HistoryURLProvider* history_url_provider() const {
    return history_url_provider_;
  }
  KeywordProvider* keyword_provider() const { return keyword_provider_; }
  SearchProvider* search_provider() const { return search_provider_; }

  const AutocompleteInput& input() const { return input_; }
  const AutocompleteResult& result() const { return result_; }
  bool done() const { return done_; }
  const ACProviders* providers() const { return &providers_; }

  const base::TimeTicks& last_time_default_match_changed() const {
    return last_time_default_match_changed_;
  }

 private:
  friend class AutocompleteProviderTest;
  FRIEND_TEST_ALL_PREFIXES(AutocompleteProviderTest,
                           RedundantKeywordsIgnoredInResult);
  FRIEND_TEST_ALL_PREFIXES(AutocompleteProviderTest, UpdateAssistedQueryStats);
  FRIEND_TEST_ALL_PREFIXES(AutocompleteProviderTest, GetDestinationURL);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void UpdateResult(bool regenerate_result,
                    bool force_notify_default_match_changed);

  
  
  
  void UpdateAssociatedKeywords(AutocompleteResult* result);

  
  
  void UpdateKeywordDescriptions(AutocompleteResult* result);

  
  
  
  void UpdateAssistedQueryStats(AutocompleteResult* result);

  
  
  void NotifyChanged(bool notify_default_match);

  
  void CheckIfDone();

  
  void StartExpireTimer();

  
  void StartStopTimer();

  AutocompleteControllerDelegate* delegate_;

  
  ACProviders providers_;

  HistoryURLProvider* history_url_provider_;

  KeywordProvider* keyword_provider_;

  SearchProvider* search_provider_;

  ZeroSuggestProvider* zero_suggest_provider_;

  
  AutocompleteInput input_;

  
  AutocompleteResult result_;

  
  
  
  
  
  
  
  
  
  
  base::TimeTicks last_time_default_match_changed_;

  
  
  base::OneShotTimer<AutocompleteController> expire_timer_;

  
  base::OneShotTimer<AutocompleteController> stop_timer_;

  
  
  const bool in_stop_timer_field_trial_;

  
  bool done_;

  
  
  bool in_start_;

  
  bool in_zero_suggest_;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(AutocompleteController);
};

#endif  
