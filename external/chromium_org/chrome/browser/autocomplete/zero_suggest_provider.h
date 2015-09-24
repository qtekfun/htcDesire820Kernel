// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_ZERO_SUGGEST_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_ZERO_SUGGEST_PROVIDER_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "chrome/browser/autocomplete/autocomplete_provider.h"
#include "chrome/browser/autocomplete/search_provider.h"
#include "net/url_request/url_fetcher_delegate.h"

class AutocompleteInput;
class GURL;
class TemplateURLService;

namespace base {
class ListValue;
class Value;
}

namespace net {
class URLFetcher;
}

class ZeroSuggestProvider : public AutocompleteProvider,
                            public net::URLFetcherDelegate {
 public:
  
  static ZeroSuggestProvider* Create(AutocompleteProviderListener* listener,
                                     Profile* profile);

  
  virtual void Start(const AutocompleteInput& input,
                     bool ) OVERRIDE;
  virtual void Stop(bool clear_cached_results) OVERRIDE;

  
  virtual void AddProviderInfo(ProvidersInfo* provider_info) const OVERRIDE;

  
  virtual void ResetSession() OVERRIDE;

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  
  
  void StartZeroSuggest(
      const GURL& curent_page_url,
      AutocompleteInput::PageClassification page_classification,
      const base::string16& permanent_text);

  bool field_trial_triggered_in_session() const {
    return field_trial_triggered_in_session_;
  }

 private:
  ZeroSuggestProvider(AutocompleteProviderListener* listener,
                      Profile* profile);

  virtual ~ZeroSuggestProvider();

  
  
  
  

  
  
  
  
  void FillResults(const base::Value& root_val,
                   int* verbatim_relevance,
                   SearchProvider::SuggestResults* suggest_results,
                   SearchProvider::NavigationResults* navigation_results);

  
  
  void AddSuggestResultsToMap(const SearchProvider::SuggestResults& results,
                              const TemplateURL* template_url,
                              SearchProvider::MatchMap* map);

  
  
  
  
  
  
  void AddMatchToMap(int relevance,
                     AutocompleteMatch::Type type,
                     const TemplateURL* template_url,
                     const base::string16& query_string,
                     int accepted_suggestion,
                     SearchProvider::MatchMap* map);

  
  AutocompleteMatch NavigationToMatch(
      const SearchProvider::NavigationResult& navigation);

  
  void Run(const GURL& suggest_url);

  
  void ParseSuggestResults(const base::Value& root_val);

  
  
  
  void ConvertResultsToAutocompleteMatches();

  
  
  
  AutocompleteMatch MatchForCurrentURL();

  
  
  
  void OnMostVisitedUrlsAvailable(const history::MostVisitedURLList& urls);

  
  TemplateURLService* template_url_service_;

  
  std::string current_query_;

  
  
  AutocompleteInput::PageClassification current_page_classification_;

  
  base::string16 permanent_text_;

  
  scoped_ptr<net::URLFetcher> fetcher_;
  
  bool have_pending_request_;

  
  AutocompleteMatch current_url_match_;
  
  SearchProvider::NavigationResults navigation_results_;
  
  SearchProvider::MatchMap query_matches_map_;
  
  int verbatim_relevance_;

  
  
  
  
  bool field_trial_triggered_;
  
  
  
  
  bool field_trial_triggered_in_session_;

  history::MostVisitedURLList most_visited_urls_;

  
  base::WeakPtrFactory<ZeroSuggestProvider> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ZeroSuggestProvider);
};

#endif  
