// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_INPUT_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_INPUT_H_

#include <string>

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/strings/string16.h"
#include "url/gurl.h"
#include "url/url_parse.h"

class AutocompleteInput {
 public:
  
  
  
  
  enum Type {
    INVALID,        
    UNKNOWN,        
    URL,            
    QUERY,          
    FORCED_QUERY,   
  };

  
  
  
  enum MatchesRequested {
    
    
    
    
    BEST_MATCH,

    
    SYNCHRONOUS_MATCHES,

    
    ALL_MATCHES,
  };

  
  
  
  
  enum PageClassification {
    
    INVALID_SPEC = 0,

    
    
    
    
    
    
    
    NTP = 1,

    
    BLANK = 2,

    
    
    
    HOME_PAGE = 3,

    
    
    OTHER = 4,

    
    
    
    SEARCH_RESULT_PAGE_DOING_SEARCH_TERM_REPLACEMENT = 6,

    
    
    INSTANT_NTP_WITH_OMNIBOX_AS_STARTING_FOCUS = 7,

    
    
    INSTANT_NTP_WITH_FAKEBOX_AS_STARTING_FOCUS = 8,

    
    
    
    SEARCH_RESULT_PAGE_NO_SEARCH_TERM_REPLACEMENT = 9
  };

  AutocompleteInput();
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  AutocompleteInput(const base::string16& text,
                    size_t cursor_position,
                    const base::string16& desired_tld,
                    const GURL& current_url,
                    PageClassification current_page_classification,
                    bool prevent_inline_autocomplete,
                    bool prefer_keyword,
                    bool allow_exact_keyword_match,
                    MatchesRequested matches_requested);
  ~AutocompleteInput();

  
  
  static size_t RemoveForcedQueryStringIfNecessary(Type type,
                                                   base::string16* text);

  
  static std::string TypeToString(Type type);

  
  
  
  
  
  static Type Parse(const base::string16& text,
                    const base::string16& desired_tld,
                    url_parse::Parsed* parts,
                    base::string16* scheme,
                    GURL* canonicalized_url);

  
  
  
  
  static void ParseForEmphasizeComponents(const base::string16& text,
                                          url_parse::Component* scheme,
                                          url_parse::Component* host);

  
  
  
  
  
  
  static base::string16 FormattedStringWithEquivalentMeaning(
      const GURL& url,
      const base::string16& formatted_url);

  
  static int NumNonHostComponents(const url_parse::Parsed& parts);

  
  static bool HasHTTPScheme(const base::string16& text);

  
  const base::string16& text() const { return text_; }

  
  
  size_t cursor_position() const { return cursor_position_; }

  
  
  
  void UpdateText(const base::string16& text,
                  size_t cursor_position,
                  const url_parse::Parsed& parts);

  
  const GURL& current_url() const { return current_url_; }

  
  
  AutocompleteInput::PageClassification current_page_classification() const {
    return current_page_classification_;
  }

  
  Type type() const { return type_; }

  
  const url_parse::Parsed& parts() const { return parts_; }

  
  
  const base::string16& scheme() const { return scheme_; }

  
  const GURL& canonicalized_url() const { return canonicalized_url_; }

  
  bool prevent_inline_autocomplete() const {
    return prevent_inline_autocomplete_;
  }

  
  
  bool prefer_keyword() const { return prefer_keyword_; }

  
  
  
  bool allow_exact_keyword_match() const { return allow_exact_keyword_match_; }

  
  MatchesRequested matches_requested() const { return matches_requested_; }

  
  void Clear();

 private:
  FRIEND_TEST_ALL_PREFIXES(AutocompleteProviderTest, GetDestinationURL);

  
  
  base::string16 text_;
  size_t cursor_position_;
  GURL current_url_;
  AutocompleteInput::PageClassification current_page_classification_;
  Type type_;
  url_parse::Parsed parts_;
  base::string16 scheme_;
  GURL canonicalized_url_;
  bool prevent_inline_autocomplete_;
  bool prefer_keyword_;
  bool allow_exact_keyword_match_;
  MatchesRequested matches_requested_;
};

#endif  
