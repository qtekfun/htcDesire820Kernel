// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_HISTORY_URL_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_HISTORY_URL_PROVIDER_H_
#pragma once

#include <string>

#include "base/compiler_specific.h"
#include "chrome/browser/autocomplete/history_provider.h"
#include "chrome/browser/autocomplete/history_provider_util.h"

class MessageLoop;
class Profile;

namespace history {

class HistoryBackend;
class URLDatabase;
class URLRow;

}  


struct HistoryURLProviderParams {
  HistoryURLProviderParams(const AutocompleteInput& input,
                           bool trim_http,
                           const std::string& languages);
  ~HistoryURLProviderParams();

  MessageLoop* message_loop;

  
  
  AutocompleteInput input;

  
  bool trim_http;

  
  
  
  
  
  bool cancel;

  
  
  
  
  
  bool failed;

  // List of matches written by the history thread.  We keep this separate list
  
  
  
  ACMatches matches;

  
  std::string languages;

  
  bool dont_suggest_exact_input;

 private:
  DISALLOW_COPY_AND_ASSIGN(HistoryURLProviderParams);
};

class HistoryURLProvider : public HistoryProvider {
 public:
  HistoryURLProvider(ACProviderListener* listener, Profile* profile);

#ifdef UNIT_TEST
  HistoryURLProvider(ACProviderListener* listener,
                     Profile* profile,
                     const std::string& languages)
    : HistoryProvider(listener, profile, "History"),
      prefixes_(GetPrefixes()),
      params_(NULL),
      languages_(languages) {}
#endif
  

  
  virtual void Start(const AutocompleteInput& input,
                     bool minimal_changes) OVERRIDE;
  virtual void Stop() OVERRIDE;

  
  
  
  
  void ExecuteWithDB(history::HistoryBackend* backend,
                     history::URLDatabase* db,
                     HistoryURLProviderParams* params);

  
  
  void DoAutocomplete(history::HistoryBackend* backend,
                      history::URLDatabase* db,
                      HistoryURLProviderParams* params);

  
  
  
  void QueryComplete(HistoryURLProviderParams* params_gets_deleted);

 private:
  ~HistoryURLProvider();

  
  static history::Prefixes GetPrefixes();

  
  
  
  
  static int CalculateRelevance(AutocompleteInput::Type input_type,
                                MatchType match_type,
                                size_t match_number);

  
  
  
  static GURL ConvertToHostOnly(const history::HistoryMatch& match,
                                const string16& input);

  
  
  
  
  
  
  
  static void PromoteOrCreateShorterSuggestion(
      history::URLDatabase* db,
      const HistoryURLProviderParams& params,
      bool have_what_you_typed_match,
      const AutocompleteMatch& what_you_typed_match,
      history::HistoryMatches* matches);

  
  
  
  
  
  
  
  static void EnsureMatchPresent(const history::URLRow& info,
                                 string16::size_type input_location,
                                 bool match_in_scheme,
                                 history::HistoryMatches* matches,
                                 bool promote);

  
  void RunAutocompletePasses(const AutocompleteInput& input,
                             bool fixup_input_and_run_pass_1);

  
  
  
  
  
  
  const history::Prefix* BestPrefix(const GURL& text,
                                    const string16& prefix_suffix) const;

  
  AutocompleteMatch SuggestExactInput(const AutocompleteInput& input,
                                      bool trim_http);

  
  
  
  
  
  
  bool FixupExactSuggestion(history::URLDatabase* db,
                            const AutocompleteInput& input,
                            AutocompleteMatch* match,
                            history::HistoryMatches* matches) const;

  
  
  bool PromoteMatchForInlineAutocomplete(HistoryURLProviderParams* params,
                                         const history::HistoryMatch& match);

  
  void SortMatches(history::HistoryMatches* matches) const;

  
  
  
  void CullPoorMatches(history::HistoryMatches* matches) const;

  
  
  void CullRedirects(history::HistoryBackend* backend,
                     history::HistoryMatches* matches,
                     size_t max_results) const;

  
  
  
  
  
  
  
  
  size_t RemoveSubsequentMatchesOf(history::HistoryMatches* matches,
                                   size_t source_index,
                                   const std::vector<GURL>& remove) const;

  
  AutocompleteMatch HistoryMatchToACMatch(
      HistoryURLProviderParams* params,
      const history::HistoryMatch& history_match,
      MatchType match_type,
      size_t match_number);

  
  const history::Prefixes prefixes_;

  
  
  
  
  HistoryURLProviderParams* params_;

  
  
  std::string languages_;
};

#endif  
