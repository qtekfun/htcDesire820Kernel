// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_TOKENIZED_STRING_MATCH_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_TOKENIZED_STRING_MATCH_H_

#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "ui/gfx/range/range.h"

namespace app_list {

class TokenizedString;

class TokenizedStringMatch {
 public:
  typedef std::vector<gfx::Range> Hits;

  TokenizedStringMatch();
  ~TokenizedStringMatch();

  
  
  bool Calculate(const TokenizedString& query, const TokenizedString& text);

  
  bool Calculate(const base::string16& query, const base::string16& text);

  double relevance() const { return relevance_; }
  const Hits& hits() const { return hits_; }

 private:
  double relevance_;
  Hits hits_;

  DISALLOW_COPY_AND_ASSIGN(TokenizedStringMatch);
};

}  

#endif  
