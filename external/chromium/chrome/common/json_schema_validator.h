// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_JSON_SCHEMA_VALIDATOR_H_
#define CHROME_COMMON_JSON_SCHEMA_VALIDATOR_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"

class DictionaryValue;
class FundamentalValue;
class ListValue;
class StringValue;
class Value;

class JSONSchemaValidator {
 public:
  
  struct Error {
    Error();

    explicit Error(const std::string& message);

    Error(const std::string& path, const std::string& message);

    
    std::string path;

    
    std::string message;
  };

  
  static const char kUnknownTypeReference[];
  static const char kInvalidChoice[];
  static const char kInvalidEnum[];
  static const char kObjectPropertyIsRequired[];
  static const char kUnexpectedProperty[];
  static const char kArrayMinItems[];
  static const char kArrayMaxItems[];
  static const char kArrayItemRequired[];
  static const char kStringMinLength[];
  static const char kStringMaxLength[];
  static const char kStringPattern[];
  static const char kNumberMinimum[];
  static const char kNumberMaximum[];
  static const char kInvalidType[];

  
  static std::string GetJSONSchemaType(Value* value);

  
  
  
  static std::string FormatErrorMessage(const std::string& format,
                                        const std::string& s1);
  static std::string FormatErrorMessage(const std::string& format,
                                        const std::string& s1,
                                        const std::string& s2);

  
  
  
  
  
  explicit JSONSchemaValidator(DictionaryValue* schema);

  
  
  
  
  
  
  
  
  JSONSchemaValidator(DictionaryValue* schema, ListValue* types);

  ~JSONSchemaValidator();

  
  
  
  
  
  
  
  
  bool default_allow_additional_properties() const {
    return default_allow_additional_properties_;
  }

  void set_default_allow_additional_properties(bool val) {
    default_allow_additional_properties_ = val;
  }

  
  const std::vector<Error>& errors() const {
    return errors_;
  }

  
  
  
  bool Validate(Value* instance);

 private:
  typedef std::map<std::string, DictionaryValue*> TypeMap;

  
  
  

  
  
  
  void Validate(Value* instance, DictionaryValue* schema,
                const std::string& path);

  
  
  void ValidateChoices(Value* instance, ListValue* choices,
                       const std::string& path);

  
  void ValidateEnum(Value* instance, ListValue* choices,
                    const std::string& path);

  
  void ValidateObject(DictionaryValue* instance, DictionaryValue* schema,
                      const std::string& path);

  
  void ValidateArray(ListValue* instance, DictionaryValue* schema,
                     const std::string& path);

  
  
  
  void ValidateTuple(ListValue* instance, DictionaryValue* schema,
                     const std::string& path);

  
  void ValidateString(StringValue* instance, DictionaryValue* schema,
                      const std::string& path);

  
  void ValidateNumber(Value* instance, DictionaryValue* schema,
                      const std::string& path);

  
  bool ValidateType(Value* instance, const std::string& expected_type,
                    const std::string& path);

  
  bool SchemaAllowsAnyAdditionalItems(
      DictionaryValue* schema, DictionaryValue** addition_items_schema);

  
  DictionaryValue* schema_root_;

  
  TypeMap types_;

  
  
  bool default_allow_additional_properties_;

  
  std::vector<Error> errors_;


  DISALLOW_COPY_AND_ASSIGN(JSONSchemaValidator);
};

#endif  
