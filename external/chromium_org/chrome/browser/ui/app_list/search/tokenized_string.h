// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_TOKENIZED_STRING_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_TOKENIZED_STRING_H_

#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "ui/gfx/range/range.h"

namespace app_list {

class TokenizedString {
 public:
  typedef std::vector<base::string16> Tokens;
  typedef std::vector<gfx::Range> Mappings;

  explicit TokenizedString(const base::string16& text);
  ~TokenizedString();

  const base::string16& text() const { return text_; }
  const Tokens& tokens() const { return tokens_; }
  const Mappings& mappings() const { return mappings_; }

 private:
  void Tokenize();

  
  const base::string16 text_;

  
  Tokens tokens_;
  Mappings mappings_;

  DISALLOW_COPY_AND_ASSIGN(TokenizedString);
};

}  

#endif  
