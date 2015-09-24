// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_TOKENIZER_H_
#define TOOLS_GN_TOKENIZER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "tools/gn/err.h"
#include "tools/gn/token.h"

class InputFile;

class Tokenizer {
 public:
  static std::vector<Token> Tokenize(const InputFile* input_file, Err* err);

  
  
  
  
  
  
  
  static size_t ByteOffsetOfNthLine(const base::StringPiece& buf, int n);

  
  
  static bool IsNewline(const base::StringPiece& buffer, size_t offset);

  static bool IsIdentifierFirstChar(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
  }

  static bool IsIdentifierContinuingChar(char c) {
    
    return IsIdentifierFirstChar(c) || (c >= '0' && c <= '9');
  }

 private:
  
  explicit Tokenizer(const InputFile* input_file, Err* err);
  ~Tokenizer();

  std::vector<Token> Run();

  void AdvanceToNextToken();
  Token::Type ClassifyCurrent() const;
  void AdvanceToEndOfToken(const Location& location, Token::Type type);

  bool IsCurrentWhitespace() const;
  bool IsCurrentNewline() const;
  bool IsCurrentStringTerminator(char quote_char) const;

  bool CanIncrement() const { return cur_ < input_.size(); }

  
  void Advance();

  
  Location GetCurrentLocation() const;

  Err GetErrorForInvalidToken(const Location& location) const;

  bool done() const { return at_end() || has_error(); }

  bool at_end() const { return cur_ == input_.size(); }
  char cur_char() const { return input_[cur_]; }

  bool has_error() const { return err_->has_error(); }

  std::vector<Token> tokens_;

  const InputFile* input_file_;
  const base::StringPiece input_;
  Err* err_;
  size_t cur_;  

  int line_number_;
  int char_in_line_;

  DISALLOW_COPY_AND_ASSIGN(Tokenizer);
};

#endif  
