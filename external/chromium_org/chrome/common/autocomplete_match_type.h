// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_AUTOCOMPLETE_MATCH_TYPE_H_
#define CHROME_COMMON_AUTOCOMPLETE_MATCH_TYPE_H_

#include <string>

struct AutocompleteMatchType {
  
  
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
                                 
                                 
    SEARCH_SUGGEST_ENTITY,       
    SEARCH_SUGGEST_INFINITE,     
                                 
    SEARCH_SUGGEST_PERSONALIZED, 
    SEARCH_SUGGEST_PROFILE,      
                                 
    SEARCH_OTHER_ENGINE,         
    EXTENSION_APP,               
                                 
    CONTACT,                     
    BOOKMARK_TITLE,              
    NUM_TYPES,
  };

  
  static std::string ToString(AutocompleteMatchType::Type type);
};

#endif  
