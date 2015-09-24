// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_SEARCH_TYPES_H_
#define CHROME_COMMON_SEARCH_TYPES_H_

struct SearchMode {
  
  enum Type {
    
    MODE_DEFAULT,

    
    MODE_NTP,

    
    MODE_SEARCH_SUGGESTIONS,

    
    MODE_SEARCH_RESULTS,
  };

  
  enum Origin {
    
    ORIGIN_DEFAULT = 0,

    
    ORIGIN_NTP,

    
    ORIGIN_SEARCH,
  };

  SearchMode() : mode(MODE_DEFAULT), origin(ORIGIN_DEFAULT) {
  }

  SearchMode(Type in_mode, Origin in_origin)
      : mode(in_mode),
        origin(in_origin) {
  }

  bool operator==(const SearchMode& rhs) const {
    return mode == rhs.mode && origin == rhs.origin;
  }

  bool operator!=(const SearchMode& rhs) const {
    return !(*this == rhs);
  }

  bool is_default() const {
    return mode == MODE_DEFAULT;
  }

  bool is_ntp() const {
    return mode == MODE_NTP;
  }

  bool is_search() const {
    return mode == MODE_SEARCH_SUGGESTIONS || mode == MODE_SEARCH_RESULTS;
  }

  bool is_search_results() const {
    return mode == MODE_SEARCH_RESULTS;
  }

  bool is_search_suggestions() const {
    return mode == MODE_SEARCH_SUGGESTIONS;
  }

  bool is_origin_default() const {
    return origin == ORIGIN_DEFAULT;
  }

  bool is_origin_search() const {
    return origin == ORIGIN_SEARCH;
  }

  bool is_origin_ntp() const {
    return origin == ORIGIN_NTP;
  }

  Type mode;
  Origin origin;
};

#endif  
