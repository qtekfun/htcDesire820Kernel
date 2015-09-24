// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_JSON_JSON_PARSER_H_
#define BASE_JSON_JSON_PARSER_H_

#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/json/json_reader.h"
#include "base/strings/string_piece.h"

#if !defined(OS_CHROMEOS)
#include "base/gtest_prod_util.h"
#endif

namespace base {
class Value;
}

#if defined(OS_CHROMEOS)
#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test

#define FRIEND_TEST_ALL_PREFIXES(test_case_name, test_name) \
  FRIEND_TEST(test_case_name, test_name); \
  FRIEND_TEST(test_case_name, DISABLED_##test_name); \
  FRIEND_TEST(test_case_name, FLAKY_##test_name)
#endif  

namespace base {
namespace internal {

class JSONParserTest;

class BASE_EXPORT_PRIVATE JSONParser {
 public:
  explicit JSONParser(int options);
  ~JSONParser();

  
  
  Value* Parse(const StringPiece& input);

  
  JSONReader::JsonParseError error_code() const;

  
  std::string GetErrorMessage() const;

 private:
  enum Token {
    T_OBJECT_BEGIN,           
    T_OBJECT_END,             
    T_ARRAY_BEGIN,            
    T_ARRAY_END,              
    T_STRING,
    T_NUMBER,
    T_BOOL_TRUE,              
    T_BOOL_FALSE,             
    T_NULL,                   
    T_LIST_SEPARATOR,         
    T_OBJECT_PAIR_SEPARATOR,  
    T_END_OF_INPUT,
    T_INVALID_TOKEN,
  };

  
  
  
  
  
  class StringBuilder {
   public:
    
    StringBuilder();

    
    explicit StringBuilder(const char* pos);

    ~StringBuilder();

    
    void Swap(StringBuilder* other);

    
    
    
    
    void Append(const char& c);

    
    void AppendString(const std::string& str);

    
    
    
    void Convert();

    
    bool CanBeStringPiece() const;

    
    
    StringPiece AsStringPiece();

    
    const std::string& AsString();

   private:
    
    const char* pos_;

    
    size_t length_;

    
    
    std::string* string_;
  };

  
  bool CanConsume(int length);

  
  
  const char* NextChar();

  
  void NextNChars(int n);

  
  
  Token GetNextToken();

  
  
  void EatWhitespaceAndComments();
  
  
  bool EatComment();

  
  Value* ParseNextToken();

  
  
  
  Value* ParseToken(Token token);

  
  
  Value* ConsumeDictionary();

  
  
  Value* ConsumeList();

  
  Value* ConsumeString();

  
  
  
  
  bool ConsumeStringRaw(StringBuilder* out);
  
  
  
  
  
  bool DecodeUTF16(std::string* dest_string);
  
  
  
  void DecodeUTF8(const int32& point, StringBuilder* dest);

  
  
  Value* ConsumeNumber();
  
  
  bool ReadInt(bool allow_leading_zeros);

  
  
  Value* ConsumeLiteral();

  
  static bool StringsAreEqual(const char* left, const char* right, size_t len);

  
  
  
  void ReportError(JSONReader::JsonParseError code, int column_adjust);

  
  
  static std::string FormatErrorMessage(int line, int column,
                                        const std::string& description);

  
  int options_;

  
  const char* start_pos_;

  
  
  const char* pos_;

  
  const char* end_pos_;

  
  int index_;

  
  int stack_depth_;

  
  int line_number_;

  
  int index_last_line_;

  
  JSONReader::JsonParseError error_code_;
  int error_line_;
  int error_column_;

  friend class JSONParserTest;
  FRIEND_TEST_ALL_PREFIXES(JSONParserTest, NextChar);
  FRIEND_TEST_ALL_PREFIXES(JSONParserTest, ConsumeDictionary);
  FRIEND_TEST_ALL_PREFIXES(JSONParserTest, ConsumeList);
  FRIEND_TEST_ALL_PREFIXES(JSONParserTest, ConsumeString);
  FRIEND_TEST_ALL_PREFIXES(JSONParserTest, ConsumeLiterals);
  FRIEND_TEST_ALL_PREFIXES(JSONParserTest, ConsumeNumbers);
  FRIEND_TEST_ALL_PREFIXES(JSONParserTest, ErrorMessages);

  DISALLOW_COPY_AND_ASSIGN(JSONParser);
};

}  
}  

#endif  
