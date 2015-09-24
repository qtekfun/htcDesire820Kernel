// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_HISTORY_CONTENTS_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_HISTORY_CONTENTS_PROVIDER_H_
#pragma once

#include "chrome/browser/autocomplete/history_provider.h"
#include "chrome/browser/history/history.h"

namespace bookmark_utils {
struct TitleMatch;
}

class HistoryContentsProvider : public HistoryProvider {
 public:
  HistoryContentsProvider(ACProviderListener* listener, Profile* profile);

  
  
  virtual void Start(const AutocompleteInput& input,
                     bool minimal_changes) OVERRIDE;
  virtual void Stop() OVERRIDE;

 private:
  ~HistoryContentsProvider();

  void QueryComplete(HistoryService::Handle handle,
                     history::QueryResults* results);

  
  
  void ConvertResults();

  
  AutocompleteMatch ResultToMatch(const history::URLResult& result,
                                  int score);

  
  
  void ClassifyDescription(const history::URLResult& result,
                           AutocompleteMatch* match) const;

  
  
  int CalculateRelevance(const history::URLResult& result);

  
  
  void QueryBookmarks(const AutocompleteInput& input);

  
  
  void AddBookmarkTitleMatchToResults(const bookmark_utils::TitleMatch& match);

  CancelableRequestConsumerT<int, 0> request_consumer_;

  
  
  int star_title_count_;
  int star_contents_count_;
  int title_count_;
  int contents_count_;

  
  AutocompleteInput::Type input_type_;

  
  bool trim_http_;

  
  
  history::QueryResults results_;

  
  bool have_results_;

  
  string16 query_;

  DISALLOW_COPY_AND_ASSIGN(HistoryContentsProvider);
};

#endif  
