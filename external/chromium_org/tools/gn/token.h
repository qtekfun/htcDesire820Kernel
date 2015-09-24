// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_TOKEN_H_
#define TOOLS_GN_TOKEN_H_

#include "base/strings/string_piece.h"
#include "tools/gn/location.h"

class Token {
 public:
  enum Type {
    INVALID,
    INTEGER,    
    STRING,     
    TRUE_TOKEN,  
    FALSE_TOKEN,

    
    EQUAL,
    PLUS,
    MINUS,
    PLUS_EQUALS,
    MINUS_EQUALS,
    EQUAL_EQUAL,
    NOT_EQUAL,
    LESS_EQUAL,
    GREATER_EQUAL,
    LESS_THAN,
    GREATER_THAN,
    BOOLEAN_AND,
    BOOLEAN_OR,
    BANG,

    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,

    IF,
    ELSE,
    IDENTIFIER, 
    COMMA,  
    COMMENT,    

    UNCLASSIFIED_OPERATOR,  

    NUM_TYPES
  };

  Token();
  Token(const Location& location, Type t, const base::StringPiece& v);

  Type type() const { return type_; }
  const base::StringPiece& value() const { return value_; }
  const Location& location() const { return location_; }
  LocationRange range() const {
    return LocationRange(location_,
                         Location(location_.file(), location_.line_number(),
                                  location_.char_offset() +
                                      static_cast<int>(value_.size())));
  }

  
  bool IsIdentifierEqualTo(const char* v) const;
  bool IsStringEqualTo(const char* v) const;

  
  
  std::string StringValue() const;

 private:
  Type type_;
  base::StringPiece value_;
  Location location_;
};

#endif  
