// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_HISTORY_PROVIDER_UTIL_H_
#define CHROME_BROWSER_AUTOCOMPLETE_HISTORY_PROVIDER_UTIL_H_
#pragma once

#include <deque>
#include <vector>

#include "chrome/browser/history/history_types.h"

namespace history {

struct HistoryMatch {
  
  HistoryMatch();

  HistoryMatch(const URLRow& url_info,
               size_t input_location,
               bool match_in_scheme,
               bool innermost_match);

  bool operator==(const GURL& url) const;

  URLRow url_info;

  
  size_t input_location;

  
  
  
  
  
  
  
  
  bool match_in_scheme;

  
  
  
  
  
  bool innermost_match;
};
typedef std::deque<HistoryMatch> HistoryMatches;

struct Prefix {
  Prefix(const string16& prefix, int num_components)
      : prefix(prefix),
        num_components(num_components) {}

  string16 prefix;

  
  
  
  
  
  int num_components;
};
typedef std::vector<Prefix> Prefixes;
}

#endif  
