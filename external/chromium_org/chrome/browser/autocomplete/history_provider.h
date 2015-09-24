// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_HISTORY_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_HISTORY_PROVIDER_H_

#include "base/compiler_specific.h"
#include "chrome/browser/autocomplete/autocomplete_provider.h"
#include "chrome/browser/history/in_memory_url_index_types.h"

class AutocompleteInput;
struct AutocompleteMatch;

class HistoryProvider : public AutocompleteProvider {
 public:
  virtual void DeleteMatch(const AutocompleteMatch& match) OVERRIDE;

 protected:
  HistoryProvider(AutocompleteProviderListener* listener,
                  Profile* profile,
                  AutocompleteProvider::Type type);
  virtual ~HistoryProvider();

  
  
  void DeleteMatchFromMatches(const AutocompleteMatch& match);

  
  
  
  
  
  bool PreventInlineAutocomplete(const AutocompleteInput& input);

  
  
  static ACMatchClassifications SpansFromTermMatch(
      const history::TermMatches& matches,
      size_t text_length,
      bool is_url);
};

#endif  
