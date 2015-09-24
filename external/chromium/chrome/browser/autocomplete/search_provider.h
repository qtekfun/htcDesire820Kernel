// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_SEARCH_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_SEARCH_PROVIDER_H_
#pragma once

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/autocomplete/autocomplete.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/search_engines/template_url.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "chrome/common/net/url_fetcher.h"

class Profile;
class Value;

class SearchProvider : public AutocompleteProvider,
                       public URLFetcher::Delegate {
 public:
  SearchProvider(ACProviderListener* listener, Profile* profile);

#if defined(UNIT_TEST)
  static void set_query_suggest_immediately(bool value) {
    query_suggest_immediately_ = value;
  }
#endif

  
  
  
  
  
  
  
  void FinalizeInstantQuery(const string16& input_text,
                            const string16& suggest_text);

  
  virtual void Start(const AutocompleteInput& input,
                     bool minimal_changes);
  virtual void Stop();

  
  virtual void OnURLFetchComplete(const URLFetcher* source,
                                  const GURL& url,
                                  const net::URLRequestStatus& status,
                                  int response_code,
                                  const ResponseCookies& cookies,
                                  const std::string& data);

  
  static const int kDefaultProviderURLFetcherID;

  
  static const int kKeywordProviderURLFetcherID;

 private:
  ~SearchProvider();

  
  
  
  
  
  
  class Providers {
   public:
    Providers() : default_provider_(NULL), keyword_provider_(NULL) {}

    
    
    bool equals(const TemplateURL* default_provider,
                const TemplateURL* keyword_provider) {
      return (default_provider == default_provider_ &&
              keyword_provider == keyword_provider_);
    }

    
    void Set(const TemplateURL* default_provider,
             const TemplateURL* keyword_provider);

    const TemplateURL& default_provider() const {
      DCHECK(valid_default_provider());
      return cached_default_provider_;
    }

    const TemplateURL& keyword_provider() const {
      DCHECK(valid_keyword_provider());
      return cached_keyword_provider_;
    }

    
    bool valid_keyword_provider() const { return !!keyword_provider_; }

    
    
    bool valid_suggest_for_keyword_provider() const {
      return keyword_provider_ && cached_keyword_provider_.suggestions_url();
    }

    
    bool valid_default_provider() const { return !!default_provider_; }

    
    
    bool valid_suggest_for_default_provider() const {
      return default_provider_ && cached_default_provider_.suggestions_url();
    }

    
    
    bool is_primary_provider(bool from_keyword_provider) const {
      return from_keyword_provider || !valid_keyword_provider();
    }

   private:
    
    
    TemplateURL cached_default_provider_;
    TemplateURL cached_keyword_provider_;

    
    const TemplateURL* default_provider_;
    const TemplateURL* keyword_provider_;
  };

  struct NavigationResult {
    NavigationResult(const GURL& url, const string16& site_name)
        : url(url),
          site_name(site_name) {
    }

    
    GURL url;

    
    string16 site_name;
  };

  typedef std::vector<string16> SuggestResults;
  typedef std::vector<NavigationResult> NavigationResults;
  typedef std::vector<history::KeywordSearchTermVisit> HistoryResults;
  typedef std::map<string16, AutocompleteMatch> MatchMap;

  
  void Run();

  
  
  void DoHistoryQuery(bool minimal_changes);

  
  
  
  void StartOrStopSuggestQuery(bool minimal_changes);

  
  
  
  bool IsQuerySuitableForSuggest() const;

  
  
  void StopSuggest();

  
  
  URLFetcher* CreateSuggestFetcher(int id,
                                   const TemplateURL& provider,
                                   const string16& text);

  
  
  bool ParseSuggestResults(Value* root_val,
                           bool is_keyword,
                           const string16& input_text,
                           SuggestResults* suggest_results);

  
  
  
  void ConvertResultsToAutocompleteMatches();

  
  
  void AddNavigationResultsToMatches(
    const NavigationResults& navigation_results,
    bool is_keyword);

  
  
  void AddHistoryResultsToMap(const HistoryResults& results,
                              bool is_keyword,
                              int did_not_accept_suggestion,
                              MatchMap* map);

  
  
  
  void AddSuggestResultsToMap(const SuggestResults& suggest_results,
                              bool is_keyword,
                              int did_not_accept_suggestion,
                              MatchMap* map);

  
  
  int CalculateRelevanceForWhatYouTyped() const;
  
  
  
  int CalculateRelevanceForHistory(const base::Time& time,
                                   bool looks_like_url,
                                   bool is_keyword) const;
  
  
  
  int CalculateRelevanceForSuggestion(size_t num_results,
                                      size_t result_number,
                                      bool is_keyword) const;
  
  
  int CalculateRelevanceForNavigation(size_t num_results,
                                      size_t result_number,
                                      bool is_keyword) const;

  
  
  
  void AddMatchToMap(const string16& query_string,
                     const string16& input_text,
                     int relevance,
                     AutocompleteMatch::Type type,
                     int accepted_suggestion,
                     bool is_keyword,
                     bool prevent_inline_autocomplete,
                     MatchMap* map);

  
  AutocompleteMatch NavigationToMatch(const NavigationResult& query_string,
                                      int relevance,
                                      bool is_keyword);

  
  void UpdateDone();

  
  void UpdateFirstSearchMatchDescription();

  
  
  static bool query_suggest_immediately_;

  
  Providers providers_;

  
  AutocompleteInput input_;

  
  string16 keyword_input_text_;

  
  HistoryResults keyword_history_results_;
  HistoryResults default_history_results_;

  
  
  int suggest_results_pending_;

  
  
  base::OneShotTimer<SearchProvider> timer_;

  
  scoped_ptr<URLFetcher> keyword_fetcher_;

  
  
  scoped_ptr<URLFetcher> default_fetcher_;

  
  SuggestResults keyword_suggest_results_;
  SuggestResults default_suggest_results_;

  
  NavigationResults keyword_navigation_results_;
  NavigationResults default_navigation_results_;

  
  bool have_suggest_results_;

  
  bool instant_finalized_;

  
  string16 default_provider_suggest_text_;

  DISALLOW_COPY_AND_ASSIGN(SearchProvider);
};

#endif  
