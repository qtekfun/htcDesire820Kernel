// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_AUTOCOMPLETE_HISTORY_PROVIDER_UTIL_H_
#define CHROME_BROWSER_AUTOCOMPLETE_HISTORY_PROVIDER_UTIL_H_

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

  static bool EqualsGURL(const HistoryMatch& h, const GURL& url);

  
  
  
  bool IsHostOnly() const;

  URLRow url_info;

  
  size_t input_location;

  
  
  
  
  
  
  
  
  bool match_in_scheme;

  
  
  
  
  
  bool innermost_match;

  
  
  bool promoted;
};
typedef std::deque<HistoryMatch> HistoryMatches;

}  

#endif  
