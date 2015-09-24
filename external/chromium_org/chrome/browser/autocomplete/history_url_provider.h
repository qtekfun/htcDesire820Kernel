// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_HISTORY_URL_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_HISTORY_URL_PROVIDER_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/synchronization/cancellation_flag.h"
#include "chrome/browser/autocomplete/autocomplete_input.h"
#include "chrome/browser/autocomplete/history_provider.h"
#include "chrome/browser/autocomplete/history_provider_util.h"
#include "chrome/browser/autocomplete/url_prefix.h"
#include "chrome/browser/search_engines/search_terms_data.h"
#include "chrome/browser/search_engines/template_url.h"

class Profile;

namespace base {
class MessageLoop;
}

namespace history {
class HistoryBackend;
class URLDatabase;
}


struct HistoryURLProviderParams {
  HistoryURLProviderParams(const AutocompleteInput& input,
                           bool trim_http,
                           const std::string& languages,
                           TemplateURL* default_search_provider,
                           const SearchTermsData& search_terms_data);
  ~HistoryURLProviderParams();

  base::MessageLoop* message_loop;

  
  
  AutocompleteInput input;

  
  
  
  bool prevent_inline_autocomplete;

  
  bool trim_http;

  
  
  
  
  base::CancellationFlag cancel_flag;

  
  
  
  
  
  bool failed;

  // List of matches written by the history thread.  We keep this separate list
  
  
  
  ACMatches matches;

  
  std::string languages;

  
  bool dont_suggest_exact_input;

  
  
  
  
  
  
  
  scoped_ptr<TemplateURL> default_search_provider;
  scoped_ptr<SearchTermsData> search_terms_data;

 private:
  DISALLOW_COPY_AND_ASSIGN(HistoryURLProviderParams);
};

class HistoryURLProvider : public HistoryProvider {
 public:
  
  
  static const int kScoreForBestInlineableResult;
  static const int kScoreForUnvisitedIntranetResult;
  static const int kScoreForWhatYouTypedResult;
  static const int kBaseScoreForNonInlineableResult;

  HistoryURLProvider(AutocompleteProviderListener* listener, Profile* profile);

  
  virtual void Start(const AutocompleteInput& input,
                     bool minimal_changes) OVERRIDE;
  virtual void Stop(bool clear_cached_results) OVERRIDE;

  
  
  
  
  
  
  AutocompleteMatch SuggestExactInput(const base::string16& text,
                                      const GURL& destination_url,
                                      bool trim_http);

  
  
  
  
  void ExecuteWithDB(history::HistoryBackend* backend,
                     history::URLDatabase* db,
                     HistoryURLProviderParams* params);

  
  
  void DoAutocomplete(history::HistoryBackend* backend,
                      history::URLDatabase* db,
                      HistoryURLProviderParams* params);

  
  
  
  void QueryComplete(HistoryURLProviderParams* params_gets_deleted);

 private:
  enum MatchType {
    NORMAL,
    WHAT_YOU_TYPED,
    INLINE_AUTOCOMPLETE,
    UNVISITED_INTRANET,  
  };
  class VisitClassifier;

  ~HistoryURLProvider();

  
  
  
  
  
  
  
  int CalculateRelevance(MatchType match_type, size_t match_number) const;

  
  void RunAutocompletePasses(const AutocompleteInput& input,
                             bool fixup_input_and_run_pass_1);

  
  
  
  
  
  
  bool FixupExactSuggestion(history::URLDatabase* db,
                            const AutocompleteInput& input,
                            const VisitClassifier& classifier,
                            AutocompleteMatch* match,
                            history::HistoryMatches* matches) const;

  
  
  
  bool CanFindIntranetURL(history::URLDatabase* db,
                          const AutocompleteInput& input) const;

  
  
  
  bool PromoteMatchForInlineAutocomplete(const history::HistoryMatch& match,
                                         HistoryURLProviderParams* params);

  
  
  
  
  
  
  void PromoteOrCreateShorterSuggestion(
      history::URLDatabase* db,
      const HistoryURLProviderParams& params,
      bool have_what_you_typed_match,
      const AutocompleteMatch& what_you_typed_match,
      history::HistoryMatches* matches);

  
  
  
  
  
  
  void CullPoorMatches(const HistoryURLProviderParams& params,
                       history::HistoryMatches* matches) const;

  
  
  void CullRedirects(history::HistoryBackend* backend,
                     history::HistoryMatches* matches,
                     size_t max_results) const;

  
  
  
  
  
  
  
  
  size_t RemoveSubsequentMatchesOf(history::HistoryMatches* matches,
                                   size_t source_index,
                                   const std::vector<GURL>& remove) const;

  
  AutocompleteMatch HistoryMatchToACMatch(
      const HistoryURLProviderParams& params,
      const history::HistoryMatch& history_match,
      MatchType match_type,
      int relevance);

  
  
  static ACMatchClassifications ClassifyDescription(
      const base::string16& input_text,
      const base::string16& description);

  
  
  
  
  HistoryURLProviderParams* params_;

  
  
  
  
  
  bool cull_redirects_;

  
  
  
  
  bool create_shorter_match_;

  
  
  
  
  
  
  
  
  bool search_url_database_;
};

#endif  
