// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_JSON_JSON_READER_H_
#define BASE_JSON_JSON_READER_H_

#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"

namespace base {
class Value;

namespace internal {
class JSONParser;
}
}

namespace base {

enum JSONParserOptions {
  
  
  JSON_PARSE_RFC = 0,

  
  JSON_ALLOW_TRAILING_COMMAS = 1 << 0,

  
  
  
  
  JSON_DETACHABLE_CHILDREN = 1 << 1,
};

class BASE_EXPORT JSONReader {
 public:
  
  enum JsonParseError {
    JSON_NO_ERROR = 0,
    JSON_INVALID_ESCAPE,
    JSON_SYNTAX_ERROR,
    JSON_UNEXPECTED_TOKEN,
    JSON_TRAILING_COMMA,
    JSON_TOO_MUCH_NESTING,
    JSON_UNEXPECTED_DATA_AFTER_ROOT,
    JSON_UNSUPPORTED_ENCODING,
    JSON_UNQUOTED_DICTIONARY_KEY,
    JSON_PARSE_ERROR_COUNT
  };

  
  static const char* kInvalidEscape;
  static const char* kSyntaxError;
  static const char* kUnexpectedToken;
  static const char* kTrailingComma;
  static const char* kTooMuchNesting;
  static const char* kUnexpectedDataAfterRoot;
  static const char* kUnsupportedEncoding;
  static const char* kUnquotedDictionaryKey;

  
  JSONReader();

  
  explicit JSONReader(int options);

  ~JSONReader();

  
  
  static Value* Read(const StringPiece& json);

  
  
  
  static Value* Read(const StringPiece& json, int options);

  
  
  
  
  static Value* ReadAndReturnError(const StringPiece& json,
                                   int options,  
                                   int* error_code_out,
                                   std::string* error_msg_out);

  
  
  static std::string ErrorCodeToString(JsonParseError error_code);

  
  Value* ReadToValue(const std::string& json);

  
  
  JsonParseError error_code() const;

  
  
  std::string GetErrorMessage() const;

 private:
  scoped_ptr<internal::JSONParser> parser_;
};

}  

#endif  
