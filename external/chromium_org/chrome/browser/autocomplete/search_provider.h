// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_SEARCH_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_SEARCH_PROVIDER_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/time/time.h"
#include "base/timer/timer.h"
#include "chrome/browser/autocomplete/autocomplete_input.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/autocomplete/autocomplete_provider.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/browser/search_engines/template_url.h"
#include "net/url_request/url_fetcher_delegate.h"

class Profile;
class SearchProviderTest;
class SuggestionDeletionHandler;
class TemplateURLService;

namespace base {
class Value;
}

namespace net {
class URLFetcher;
}

class SearchProvider : public AutocompleteProvider,
                       public net::URLFetcherDelegate {
 public:
  
  static const int kDefaultProviderURLFetcherID;

  
  static const int kKeywordProviderURLFetcherID;

  
  static const int kDeletionURLFetcherID;

  SearchProvider(AutocompleteProviderListener* listener, Profile* profile);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static AutocompleteMatch CreateSearchSuggestion(
      AutocompleteProvider* autocomplete_provider,
      const AutocompleteInput& input,
      const base::string16& input_text,
      int relevance,
      AutocompleteMatch::Type type,
      bool is_keyword,
      const base::string16& match_contents,
      const base::string16& annotation,
      const TemplateURL* template_url,
      const base::string16& query_string,
      const std::string& suggest_query_params,
      int accepted_suggestion,
      int omnibox_start_margin,
      bool append_extra_query_params);

  
  
  static bool ShouldPrefetch(const AutocompleteMatch& match);

  
  
  static std::string GetSuggestMetadata(const AutocompleteMatch& match);

  
  virtual void AddProviderInfo(ProvidersInfo* provider_info) const OVERRIDE;
  virtual void DeleteMatch(const AutocompleteMatch& match) OVERRIDE;
  virtual void ResetSession() OVERRIDE;

  bool field_trial_triggered_in_session() const {
    return field_trial_triggered_in_session_;
  }

  
  void set_current_page_url(const GURL& current_page_url) {
    current_page_url_ = current_page_url;
  }

 protected:
  virtual ~SearchProvider();

 private:
  
  
  friend class SearchProviderTest;
  friend class ZeroSuggestProvider;
  FRIEND_TEST_ALL_PREFIXES(SearchProviderTest, CanSendURL);
  FRIEND_TEST_ALL_PREFIXES(SearchProviderTest, NavigationInline);
  FRIEND_TEST_ALL_PREFIXES(SearchProviderTest, NavigationInlineDomainClassify);
  FRIEND_TEST_ALL_PREFIXES(SearchProviderTest, NavigationInlineSchemeSubstring);
  FRIEND_TEST_ALL_PREFIXES(SearchProviderTest, RemoveStaleResultsTest);
  FRIEND_TEST_ALL_PREFIXES(SearchProviderTest, SuggestRelevanceExperiment);
  FRIEND_TEST_ALL_PREFIXES(SearchProviderTest, TestDeleteMatch);
  FRIEND_TEST_ALL_PREFIXES(AutocompleteProviderTest, GetDestinationURL);
  FRIEND_TEST_ALL_PREFIXES(InstantExtendedPrefetchTest, ClearPrefetchedResults);
  FRIEND_TEST_ALL_PREFIXES(InstantExtendedPrefetchTest, SetPrefetchQuery);

  
  
  
  
  
  
  class Providers {
   public:
    explicit Providers(TemplateURLService* template_url_service);

    
    
    bool equal(const base::string16& default_provider,
               const base::string16& keyword_provider) const {
      return (default_provider == default_provider_) &&
          (keyword_provider == keyword_provider_);
    }

    
    void set(const base::string16& default_provider,
             const base::string16& keyword_provider) {
      default_provider_ = default_provider;
      keyword_provider_ = keyword_provider;
    }

    TemplateURLService* template_url_service() { return template_url_service_; }
    const base::string16& default_provider() const { return default_provider_; }
    const base::string16& keyword_provider() const { return keyword_provider_; }

    
    const TemplateURL* GetDefaultProviderURL() const;
    const TemplateURL* GetKeywordProviderURL() const;

    
    bool has_keyword_provider() const { return !keyword_provider_.empty(); }

   private:
    TemplateURLService* template_url_service_;

    
    
    base::string16 default_provider_;
    base::string16 keyword_provider_;

    DISALLOW_COPY_AND_ASSIGN(Providers);
  };

  
  
  
  
  
  
  
  class Result {
   public:
    Result(bool from_keyword_provider,
           int relevance,
           bool relevance_from_server);
    virtual ~Result();

    bool from_keyword_provider() const { return from_keyword_provider_; }

    int relevance() const { return relevance_; }
    void set_relevance(int relevance) { relevance_ = relevance; }

    bool relevance_from_server() const { return relevance_from_server_; }
    void set_relevance_from_server(bool relevance_from_server) {
      relevance_from_server_ = relevance_from_server;
    }

    
    
    virtual bool IsInlineable(const base::string16& input) const = 0;

    
    
    
    
    virtual int CalculateRelevance(const AutocompleteInput& input,
                                   bool keyword_provider_requested) const = 0;

   protected:
    
    bool from_keyword_provider_;

    
    int relevance_;

   private:
    
    
    
    
    
    bool relevance_from_server_;
  };

  class SuggestResult : public Result {
   public:
    SuggestResult(const base::string16& suggestion,
                  AutocompleteMatchType::Type type,
                  const base::string16& match_contents,
                  const base::string16& annotation,
                  const std::string& suggest_query_params,
                  const std::string& deletion_url,
                  bool from_keyword_provider,
                  int relevance,
                  bool relevance_from_server,
                  bool should_prefetch);
    virtual ~SuggestResult();

    const base::string16& suggestion() const { return suggestion_; }
    AutocompleteMatchType::Type type() const { return type_; }
    const base::string16& match_contents() const { return match_contents_; }
    const base::string16& annotation() const { return annotation_; }
    const std::string& suggest_query_params() const {
      return suggest_query_params_;
    }
    const std::string& deletion_url() const { return deletion_url_; }
    bool should_prefetch() const { return should_prefetch_; }

    
    virtual bool IsInlineable(const base::string16& input) const OVERRIDE;
    virtual int CalculateRelevance(
        const AutocompleteInput& input,
        bool keyword_provider_requested) const OVERRIDE;

   private:
    
    base::string16 suggestion_;

    AutocompleteMatchType::Type type_;

    
    base::string16 match_contents_;

    
    
    
    base::string16 annotation_;

    
    std::string suggest_query_params_;

    
    
    
    std::string deletion_url_;

    
    bool should_prefetch_;
  };

  class NavigationResult : public Result {
   public:
    
    
    NavigationResult(const AutocompleteProvider& provider,
                     const GURL& url,
                     const base::string16& description,
                     bool from_keyword_provider,
                     int relevance,
                     bool relevance_from_server);
    virtual ~NavigationResult();

    const GURL& url() const { return url_; }
    const base::string16& description() const { return description_; }
    const base::string16& formatted_url() const { return formatted_url_; }

    
    virtual bool IsInlineable(const base::string16& input) const OVERRIDE;
    virtual int CalculateRelevance(
        const AutocompleteInput& input,
        bool keyword_provider_requested) const OVERRIDE;

   private:
    
    GURL url_;

    
    
    base::string16 formatted_url_;

    
    base::string16 description_;
  };

  class CompareScoredResults;

  typedef std::vector<SuggestResult> SuggestResults;
  typedef std::vector<NavigationResult> NavigationResults;
  typedef std::vector<history::KeywordSearchTermVisit> HistoryResults;
  typedef std::pair<base::string16, std::string> MatchKey;
  typedef std::map<MatchKey, AutocompleteMatch> MatchMap;
  typedef ScopedVector<SuggestionDeletionHandler> SuggestionDeletionHandlers;

  
  
  
  
  
  struct Results {
    Results();
    ~Results();

    
    
    void Clear();

    
    
    bool HasServerProvidedScores() const;

    
    SuggestResults suggest_results;

    
    NavigationResults navigation_results;

    
    
    
    int verbatim_relevance;

    
    std::string metadata;

   private:
    DISALLOW_COPY_AND_ASSIGN(Results);
  };

  
  
  static void RemoveStaleResults(const base::string16& input,
                                 int verbatim_relevance,
                                 SuggestResults* suggest_results,
                                 NavigationResults* navigation_results);

  
  
  static int CalculateRelevanceForKeywordVerbatim(AutocompleteInput::Type type,
                                                  bool prefer_keyword);

  
  virtual void Start(const AutocompleteInput& input,
                     bool minimal_changes) OVERRIDE;
  virtual void Stop(bool clear_cached_results) OVERRIDE;

  
  virtual void OnURLFetchComplete(const net::URLFetcher* source) OVERRIDE;

  
  
  void OnDeletionComplete(bool success,
                          SuggestionDeletionHandler* handler);

  
  
  
  virtual void RecordDeletionResult(bool success);

  
  void DeleteMatchFromMatches(const AutocompleteMatch& match);

  
  void Run();

  
  
  void DoHistoryQuery(bool minimal_changes);

  
  
  
  void StartOrStopSuggestQuery(bool minimal_changes);

  
  
  
  bool IsQuerySuitableForSuggest() const;

  
  
  void StopSuggest();

  
  void ClearAllResults();

  
  
  void RemoveAllStaleResults();

  
  void ApplyCalculatedRelevance();
  void ApplyCalculatedSuggestRelevance(SuggestResults* list);
  void ApplyCalculatedNavigationRelevance(NavigationResults* list);

  
  
  net::URLFetcher* CreateSuggestFetcher(int id,
                                        const TemplateURL* template_url,
                                        const AutocompleteInput& input);

  
  
  
  static scoped_ptr<base::Value> DeserializeJsonData(std::string json_data);

  
  
  
  bool ParseSuggestResults(base::Value* root_val, bool is_keyword);

  
  void ConvertResultsToAutocompleteMatches();

  
  
  
  
  
  ACMatches::const_iterator FindTopMatch(
    bool autocomplete_result_will_reorder_for_default_match) const;

  
  
  bool IsTopMatchNavigationInKeywordMode(
      bool autocomplete_result_will_reorder_for_default_match) const;
  bool HasKeywordDefaultMatchInKeywordMode() const;
  bool IsTopMatchScoreTooLow(
      bool autocomplete_result_will_reorder_for_default_match) const;
  bool IsTopMatchSearchWithURLInput(
      bool autocomplete_result_will_reorder_for_default_match) const;
  bool HasValidDefaultMatch(
      bool autocomplete_result_will_reorder_for_default_match) const;

  
  
  void UpdateMatches();

  
  
  void AddNavigationResultsToMatches(
      const NavigationResults& navigation_results,
      ACMatches* matches);

  
  
  void AddHistoryResultsToMap(const HistoryResults& results,
                              bool is_keyword,
                              int did_not_accept_suggestion,
                              MatchMap* map);

  
  SuggestResults ScoreHistoryResults(const HistoryResults& results,
                                     bool base_prevent_inline_autocomplete,
                                     bool input_multiple_words,
                                     const base::string16& input_text,
                                     bool is_keyword);

  
  void AddSuggestResultsToMap(const SuggestResults& results,
                              const std::string& metadata,
                              MatchMap* map);

  
  
  
  
  int GetVerbatimRelevance(bool* relevance_from_server) const;

  
  
  
  int CalculateRelevanceForVerbatim() const;

  
  
  
  
  
  int CalculateRelevanceForVerbatimIgnoringKeywordModeState() const;

  
  
  
  
  
  int GetKeywordVerbatimRelevance(bool* relevance_from_server) const;

  
  
  
  
  
  
  int CalculateRelevanceForHistory(const base::Time& time,
                                   bool is_keyword,
                                   bool use_aggressive_method,
                                   bool prevent_search_history_inlining) const;

  
  
  
  void AddMatchToMap(const SuggestResult& result,
                     const base::string16& input_text,
                     const std::string& metadata,
                     int accepted_suggestion,
                     MatchMap* map);

  
  AutocompleteMatch NavigationToMatch(const NavigationResult& navigation);

  
  
  
  
  
  
  
  
  void DemoteKeywordNavigationMatchesPastTopQuery();

  
  void UpdateDone();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool CanSendURL(
      const GURL& current_page_url,
      const GURL& suggest_url,
      const TemplateURL* template_url,
      AutocompleteInput::PageClassification page_classification,
      Profile* profile);

  
  
  static int kMinimumTimeBetweenSuggestQueriesMs;

  

  
  
  static const char kRelevanceFromServerKey[];

  
  static const char kShouldPrefetchKey[];

  
  
  static const char kSuggestMetadataKey[];

  
  static const char kDeletionUrlKey[];

  
  static const char kTrue[];
  static const char kFalse[];

  
  Providers providers_;

  
  AutocompleteInput input_;

  
  AutocompleteInput keyword_input_;

  
  HistoryResults keyword_history_results_;
  HistoryResults default_history_results_;

  
  
  int suggest_results_pending_;

  
  
  base::OneShotTimer<SearchProvider> timer_;

  
  base::TimeTicks time_suggest_request_sent_;

  
  scoped_ptr<net::URLFetcher> keyword_fetcher_;
  scoped_ptr<net::URLFetcher> default_fetcher_;

  
  Results default_results_;
  Results keyword_results_;

  
  
  
  SuggestionDeletionHandlers deletion_handlers_;

  
  
  
  
  
  
  bool field_trial_triggered_;

  
  
  bool field_trial_triggered_in_session_;

  
  
  bool prevent_search_history_inlining_;

  GURL current_page_url_;

  DISALLOW_COPY_AND_ASSIGN(SearchProvider);
};

#endif  
