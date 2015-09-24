// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_SHORTCUTS_PROVIDER_H_
#define CHROME_BROWSER_AUTOCOMPLETE_SHORTCUTS_PROVIDER_H_

#include <map>
#include <set>
#include <string>

#include "base/gtest_prod_util.h"
#include "chrome/browser/autocomplete/autocomplete_provider.h"
#include "chrome/browser/autocomplete/url_prefix.h"
#include "chrome/browser/history/shortcuts_backend.h"

class Profile;

namespace history {
class ShortcutsProviderTest;
}

class ShortcutsProvider
    : public AutocompleteProvider,
      public history::ShortcutsBackend::ShortcutsBackendObserver {
 public:
  ShortcutsProvider(AutocompleteProviderListener* listener, Profile* profile);

  
  
  virtual void Start(const AutocompleteInput& input,
                     bool minimal_changes) OVERRIDE;

  virtual void DeleteMatch(const AutocompleteMatch& match) OVERRIDE;

 private:
  friend class ClassifyTest;
  friend class history::ShortcutsProviderTest;

  typedef std::multimap<char16, base::string16> WordMap;

  virtual ~ShortcutsProvider();

  
  virtual void OnShortcutsLoaded() OVERRIDE;

  
  void GetMatches(const AutocompleteInput& input);

  
  
  
  
  
  
  AutocompleteMatch ShortcutToACMatch(
      const history::ShortcutsBackend::Shortcut& shortcut,
      int relevance,
      const base::string16& term_string,
      const base::string16& fixed_up_term_string,
      const bool prevent_inline_autocomplete);

  
  
  
  
  static WordMap CreateWordMapForString(const base::string16& text);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static ACMatchClassifications ClassifyAllMatchesInString(
      const base::string16& find_text,
      const WordMap& find_words,
      const base::string16& text,
      const ACMatchClassifications& original_class);

  
  
  history::ShortcutsBackend::ShortcutMap::const_iterator FindFirstMatch(
      const base::string16& keyword,
      history::ShortcutsBackend* backend);

  int CalculateScore(
      const base::string16& terms,
      const history::ShortcutsBackend::Shortcut& shortcut,
      int max_relevance);

  std::string languages_;
  bool initialized_;
};

#endif  
