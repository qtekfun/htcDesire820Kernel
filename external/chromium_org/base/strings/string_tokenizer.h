// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_STRINGS_STRING_TOKENIZER_H_
#define BASE_STRINGS_STRING_TOKENIZER_H_

#include <algorithm>
#include <string>

#include "base/strings/string_piece.h"

namespace base {

template <class str, class const_iterator>
class StringTokenizerT {
 public:
  typedef typename str::value_type char_type;

  
  enum {
    
    RETURN_DELIMS = 1 << 0,
  };

  
  
  StringTokenizerT(const str& string,
                   const str& delims) {
    Init(string.begin(), string.end(), delims);
  }

  StringTokenizerT(const_iterator string_begin,
                   const_iterator string_end,
                   const str& delims) {
    Init(string_begin, string_end, delims);
  }

  
  void set_options(int options) { options_ = options; }

  
  
  
  
  
  void set_quote_chars(const str& quotes) { quotes_ = quotes; }

  
  
  
  bool GetNext() {
    if (quotes_.empty() && options_ == 0)
      return QuickGetNext();
    else
      return FullGetNext();
  }

  
  void Reset() {
    token_end_ = start_pos_;
  }

  
  
  
  bool token_is_delim() const { return token_is_delim_; }

  
  
  const_iterator token_begin() const { return token_begin_; }
  const_iterator token_end() const { return token_end_; }
  str token() const { return str(token_begin_, token_end_); }
  base::StringPiece token_piece() const {
    return base::StringPiece(&*token_begin_,
                             std::distance(token_begin_, token_end_));
  }

 private:
  void Init(const_iterator string_begin,
            const_iterator string_end,
            const str& delims) {
    start_pos_ = string_begin;
    token_begin_ = string_begin;
    token_end_ = string_begin;
    end_ = string_end;
    delims_ = delims;
    options_ = 0;
    token_is_delim_ = false;
  }

  
  
  
  bool QuickGetNext() {
    token_is_delim_ = false;
    for (;;) {
      token_begin_ = token_end_;
      if (token_end_ == end_)
        return false;
      ++token_end_;
      if (delims_.find(*token_begin_) == str::npos)
        break;
      
    }
    while (token_end_ != end_ && delims_.find(*token_end_) == str::npos)
      ++token_end_;
    return true;
  }

  
  bool FullGetNext() {
    AdvanceState state;
    token_is_delim_ = false;
    for (;;) {
      token_begin_ = token_end_;
      if (token_end_ == end_)
        return false;
      ++token_end_;
      if (AdvanceOne(&state, *token_begin_))
        break;
      if (options_ & RETURN_DELIMS) {
        token_is_delim_ = true;
        return true;
      }
      
    }
    while (token_end_ != end_ && AdvanceOne(&state, *token_end_))
      ++token_end_;
    return true;
  }

  bool IsDelim(char_type c) const {
    return delims_.find(c) != str::npos;
  }

  bool IsQuote(char_type c) const {
    return quotes_.find(c) != str::npos;
  }

  struct AdvanceState {
    bool in_quote;
    bool in_escape;
    char_type quote_char;
    AdvanceState() : in_quote(false), in_escape(false), quote_char('\0') {}
  };

  
  bool AdvanceOne(AdvanceState* state, char_type c) {
    if (state->in_quote) {
      if (state->in_escape) {
        state->in_escape = false;
      } else if (c == '\\') {
        state->in_escape = true;
      } else if (c == state->quote_char) {
        state->in_quote = false;
      }
    } else {
      if (IsDelim(c))
        return false;
      state->in_quote = IsQuote(state->quote_char = c);
    }
    return true;
  }

  const_iterator start_pos_;
  const_iterator token_begin_;
  const_iterator token_end_;
  const_iterator end_;
  str delims_;
  str quotes_;
  int options_;
  bool token_is_delim_;
};

typedef StringTokenizerT<std::string, std::string::const_iterator>
    StringTokenizer;
typedef StringTokenizerT<std::wstring, std::wstring::const_iterator>
    WStringTokenizer;
typedef StringTokenizerT<std::string, const char*> CStringTokenizer;

}  

#endif  
