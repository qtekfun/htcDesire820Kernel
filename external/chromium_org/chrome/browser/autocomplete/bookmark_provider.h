// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_BOOKMARK_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_BOOKMARK_PROVIDER_H_

#include <string>

#include "chrome/browser/autocomplete/autocomplete_input.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "chrome/browser/autocomplete/autocomplete_provider.h"
#include "chrome/browser/history/snippet.h"

class BookmarkModel;
struct BookmarkTitleMatch;
class Profile;

class BookmarkProvider : public AutocompleteProvider {
 public:
  BookmarkProvider(AutocompleteProviderListener* listener, Profile* profile);

  
  
  
  virtual void Start(const AutocompleteInput& input,
                     bool minimal_changes) OVERRIDE;

  
  void set_bookmark_model_for_testing(BookmarkModel* bookmark_model) {
    bookmark_model_ = bookmark_model;
  }

 private:
  virtual ~BookmarkProvider();

  
  
  
  void DoAutocomplete(const AutocompleteInput& input, bool best_match);

  
  
  
  AutocompleteMatch TitleMatchToACMatch(const BookmarkTitleMatch& title_match);

  
  
  
  
  static ACMatchClassifications ClassificationsFromMatch(
      const Snippet::MatchPositions& positions,
      size_t text_length);

  BookmarkModel* bookmark_model_;

  
  std::string languages_;

  DISALLOW_COPY_AND_ASSIGN(BookmarkProvider);
};

#endif  
