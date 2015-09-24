// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_RESULT_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_RESULT_H_

#include <stddef.h>

#include <map>

#include "base/basictypes.h"
#include "chrome/browser/autocomplete/autocomplete_input.h"
#include "chrome/browser/autocomplete/autocomplete_match.h"
#include "url/gurl.h"

class AutocompleteInput;
class AutocompleteProvider;
class Profile;

class AutocompleteResult {
 public:
  typedef ACMatches::const_iterator const_iterator;
  typedef ACMatches::iterator iterator;

  
  
  struct Selection {
    Selection()
        : provider_affinity(NULL),
          is_history_what_you_typed_match(false) {
    }

    
    void Clear();

    
    bool empty() const {
      return destination_url.is_empty() && !provider_affinity &&
          !is_history_what_you_typed_match;
    }

    
    GURL destination_url;

    
    
    const AutocompleteProvider* provider_affinity;

    
    
    
    
    bool is_history_what_you_typed_match;
  };

  
  static const size_t kMaxMatches;

  
  
  static const int kLowestDefaultScore;

  AutocompleteResult();
  ~AutocompleteResult();

  
  
  void CopyOldMatches(const AutocompleteInput& input,
                      const AutocompleteResult& old_matches,
                      Profile* profile);

  
  void AppendMatches(const ACMatches& matches);

  
  
  
  void SortAndCull(const AutocompleteInput& input, Profile* profile);

  
  bool HasCopiedMatches() const;

  
  size_t size() const;
  bool empty() const;
  const_iterator begin() const;
  iterator begin();
  const_iterator end() const;
  iterator end();

  
  const AutocompleteMatch& match_at(size_t index) const;
  AutocompleteMatch* match_at(size_t index);

  
  
  const_iterator default_match() const { return default_match_; }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool ShouldHideTopMatch() const;

  
  
  
  bool TopMatchIsVerbatimAndHasNoConsecutiveVerbatimMatches() const;

  const GURL& alternate_nav_url() const { return alternate_nav_url_; }

  
  void Reset();

  void Swap(AutocompleteResult* other);

#ifndef NDEBUG
  
  void Validate() const;
#endif

  
  
  
  static GURL ComputeAlternateNavUrl(const AutocompleteInput& input,
                                     const AutocompleteMatch& match);

 private:
  friend class AutocompleteProviderTest;

  typedef std::map<AutocompleteProvider*, ACMatches> ProviderToMatches;

#if defined(OS_ANDROID)
  
  
  typedef int matches_difference_type;
#else
  typedef ACMatches::iterator::difference_type matches_difference_type;
#endif

  
  
  static bool HasMatchByDestination(const AutocompleteMatch& match,
                                    const ACMatches& matches);

  
  void CopyFrom(const AutocompleteResult& rhs);

  
  
  
  void AddMatch(AutocompleteInput::PageClassification page_classification,
                const AutocompleteMatch& match);

  
  void BuildProviderToMatches(ProviderToMatches* provider_to_matches) const;

  
  
  void MergeMatchesByProvider(
      AutocompleteInput::PageClassification page_classification,
      const ACMatches& old_matches,
      const ACMatches& new_matches);

  ACMatches matches_;

  const_iterator default_match_;

  
  
  
  
  
  
  
  GURL alternate_nav_url_;

  DISALLOW_COPY_AND_ASSIGN(AutocompleteResult);
};

#endif  
