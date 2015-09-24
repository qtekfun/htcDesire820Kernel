// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_MATCH_H_
#define CHROME_BROWSER_AUTOCOMPLETE_AUTOCOMPLETE_MATCH_H_
#pragma once

#include <vector>
#include <string>

#include "content/common/page_transition_types.h"
#include "googleurl/src/gurl.h"

class AutocompleteProvider;
class PageTransition;
class TemplateURL;


struct AutocompleteMatch {
  
  
  
  
  
  
  
  
  
  
  
  
  
  struct ACMatchClassification {
    
    
    
    enum Style {
      NONE  = 0,
      URL   = 1 << 0,  
      MATCH = 1 << 1,  
      DIM   = 1 << 2,  
    };

    ACMatchClassification(size_t offset, int style)
        : offset(offset),
          style(style) {
    }

    
    size_t offset;

    int style;
  };

  typedef std::vector<ACMatchClassification> ACMatchClassifications;

  
  enum Type {
    URL_WHAT_YOU_TYPED = 0,  
    HISTORY_URL,             
    HISTORY_TITLE,           
    HISTORY_BODY,            
    HISTORY_KEYWORD,         
    NAVSUGGEST,              
    SEARCH_WHAT_YOU_TYPED,   
                             
    SEARCH_HISTORY,          
                             
    SEARCH_SUGGEST,          
    SEARCH_OTHER_ENGINE,     
    EXTENSION_APP,           
                             
    NUM_TYPES,
  };

  AutocompleteMatch();
  AutocompleteMatch(AutocompleteProvider* provider,
                    int relevance,
                    bool deletable,
                    Type type);
  ~AutocompleteMatch();

  
  static std::string TypeToString(Type type);

  
  
  static int TypeToIcon(Type type);

  
  static bool MoreRelevant(const AutocompleteMatch& elem1,
                           const AutocompleteMatch& elem2);

  
  static bool DestinationSortFunc(const AutocompleteMatch& elem1,
                                  const AutocompleteMatch& elem2);
  static bool DestinationsEqual(const AutocompleteMatch& elem1,
                                const AutocompleteMatch& elem2);

  
  
  
  
  static void ClassifyMatchInString(const string16& find_text,
                                    const string16& text,
                                    int style,
                                    ACMatchClassifications* classifications);

  
  
  
  
  static void ClassifyLocationInString(size_t match_location,
                                       size_t match_length,
                                       size_t overall_length,
                                       int style,
                                       ACMatchClassifications* classifications);

  
  
  
  AutocompleteProvider* provider;

  
  
  
  
  
  
  
  int relevance;

  
  bool deletable;

  
  
  
  string16 fill_into_edit;

  
  
  
  size_t inline_autocomplete_offset;

  
  
  
  GURL destination_url;

  
  string16 contents;
  ACMatchClassifications contents_class;

  
  string16 description;
  ACMatchClassifications description_class;

  
  
  
  PageTransition::Type transition;

  
  
  bool is_history_what_you_typed_match;

  
  Type type;

  
  
  const TemplateURL* template_url;

  
  bool starred;

  
  bool from_previous;

#ifndef NDEBUG
  
  void Validate() const;

  
  void ValidateClassifications(
      const string16& text,
      const ACMatchClassifications& classifications) const;
#endif
};

typedef AutocompleteMatch::ACMatchClassification ACMatchClassification;
typedef std::vector<ACMatchClassification> ACMatchClassifications;

#endif  
