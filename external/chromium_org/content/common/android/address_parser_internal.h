// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_ADDRESS_PARSER_INTERNAL_H_
#define CONTENT_COMMON_ADDRESS_PARSER_INTERNAL_H_

#include <vector>

#include "base/strings/string_tokenizer.h"
#include "content/common/content_export.h"

namespace content {

namespace address_parser {

namespace internal {

struct CONTENT_EXPORT Word {
  base::string16::const_iterator begin;
  base::string16::const_iterator end;

  Word() {}
  Word(const base::string16::const_iterator& begin,
       const base::string16::const_iterator& end);
};

class CONTENT_EXPORT HouseNumberParser {
 public:
  HouseNumberParser() {}

  bool Parse(const base::string16::const_iterator& begin,
             const base::string16::const_iterator& end,
             Word* word);

 private:
  static inline bool IsPreDelimiter(char16 character);
  static inline bool IsPostDelimiter(char16 character);
  inline void RestartOnNextDelimiter();

  inline bool CheckFinished(Word* word) const;
  inline void AcceptChars(size_t num_chars);
  inline void SkipChars(size_t num_chars);
  inline void ResetState();

  
  
  base::string16::const_iterator begin_;
  base::string16::const_iterator it_;
  base::string16::const_iterator end_;

  
  size_t num_digits_;

  
  
  size_t result_chars_;

  DISALLOW_COPY_AND_ASSIGN(HouseNumberParser);
};

typedef std::vector<Word> WordList;
typedef base::StringTokenizerT<base::string16, base::string16::const_iterator>
    String16Tokenizer;

CONTENT_EXPORT bool FindStateStartingInWord(WordList* words,
                                            size_t state_first_word,
                                            size_t* state_last_word,
                                            String16Tokenizer* tokenizer,
                                            size_t* state_index);

CONTENT_EXPORT bool IsValidLocationName(const Word& word);
CONTENT_EXPORT bool IsZipValid(const Word& word, size_t state_index);
CONTENT_EXPORT bool IsZipValidForState(const Word& word, size_t state_index);

}  

}  

}  

#endif  
