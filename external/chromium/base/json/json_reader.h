// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_JSON_JSON_READER_H_
#define BASE_JSON_JSON_READER_H_
#pragma once

#include <string>

#include "base/base_api.h"
#include "base/basictypes.h"

#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test

class Value;

namespace base {

class BASE_API JSONReader {
 public:
  
  class Token {
   public:
    enum Type {
     OBJECT_BEGIN,           
     OBJECT_END,             
     ARRAY_BEGIN,            
     ARRAY_END,              
     STRING,
     NUMBER,
     BOOL_TRUE,              
     BOOL_FALSE,             
     NULL_TOKEN,             
     LIST_SEPARATOR,         
     OBJECT_PAIR_SEPARATOR,  
     END_OF_INPUT,
     INVALID_TOKEN,
    };
    Token(Type t, const wchar_t* b, int len)
      : type(t), begin(b), length(len) {}

    
    wchar_t NextChar() {
      return *(begin + length);
    }

    Type type;

    
    const wchar_t* begin;

    
    int length;
  };

  
  enum JsonParseError {
    JSON_NO_ERROR = 0,
    JSON_BAD_ROOT_ELEMENT_TYPE,
    JSON_INVALID_ESCAPE,
    JSON_SYNTAX_ERROR,
    JSON_TRAILING_COMMA,
    JSON_TOO_MUCH_NESTING,
    JSON_UNEXPECTED_DATA_AFTER_ROOT,
    JSON_UNSUPPORTED_ENCODING,
    JSON_UNQUOTED_DICTIONARY_KEY,
  };

  
  static const char* kBadRootElementType;
  static const char* kInvalidEscape;
  static const char* kSyntaxError;
  static const char* kTrailingComma;
  static const char* kTooMuchNesting;
  static const char* kUnexpectedDataAfterRoot;
  static const char* kUnsupportedEncoding;
  static const char* kUnquotedDictionaryKey;

  JSONReader();

  
  
  
  
  static Value* Read(const std::string& json, bool allow_trailing_comma);

  
  
  
  
  static Value* ReadAndReturnError(const std::string& json,
                                   bool allow_trailing_comma,
                                   int* error_code_out,
                                   std::string* error_msg_out);

  
  
  static std::string ErrorCodeToString(JsonParseError error_code);

  
  
  JsonParseError error_code() const { return error_code_; }

  
  
  std::string GetErrorMessage() const;

  
  
  
  
  
  
  
  Value* JsonToValue(const std::string& json, bool check_root,
                     bool allow_trailing_comma);

 private:
  FRIEND_TEST(JSONReaderTest, Reading);
  FRIEND_TEST(JSONReaderTest, ErrorMessages);

  static std::string FormatErrorMessage(int line, int column,
                                        const std::string& description);

  
  
  
  Value* BuildValue(bool is_root);

  
  
  
  
  Token ParseNumberToken();

  
  
  Value* DecodeNumber(const Token& token);

  
  
  
  
  Token ParseStringToken();

  
  
  Value* DecodeString(const Token& token);

  
  
  Token ParseToken();

  
  void EatWhitespaceAndComments();

  
  
  bool EatComment();

  
  bool NextStringMatch(const std::wstring& str);

  
  
  void SetErrorCode(const JsonParseError error, const wchar_t* error_pos);

  
  const wchar_t* start_pos_;

  
  const wchar_t* json_pos_;

  
  int stack_depth_;

  
  bool allow_trailing_comma_;

  
  JsonParseError error_code_;
  int error_line_;
  int error_col_;

  DISALLOW_COPY_AND_ASSIGN(JSONReader);
};

}  

#endif  
