// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_APP_LIST_SEARCH_TOKENIZED_STRING_CHAR_ITERATOR_H_
#define CHROME_BROWSER_UI_APP_LIST_SEARCH_TOKENIZED_STRING_CHAR_ITERATOR_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "chrome/browser/ui/app_list/search/tokenized_string.h"

namespace base {
namespace i18n {
class UTF16CharIterator;
}
}

namespace app_list {

class TokenizedStringCharIterator {
 public:
  
  explicit TokenizedStringCharIterator(const TokenizedString& tokenized);
  ~TokenizedStringCharIterator();

  
  bool NextChar();

  
  
  bool NextToken();

  
  int32 Get() const;

  
  
  int32 GetArrayPos() const;

  
  size_t GetCharSize() const;

  
  bool IsFirstCharOfToken() const;

  
  bool end() const { return !current_token_iter_; }

 private:
  void CreateTokenCharIterator();

  const TokenizedString::Tokens& tokens_;
  const TokenizedString::Mappings& mappings_;

  size_t current_token_;
  scoped_ptr<base::i18n::UTF16CharIterator> current_token_iter_;

  DISALLOW_COPY_AND_ASSIGN(TokenizedStringCharIterator);
};

}  

#endif  
