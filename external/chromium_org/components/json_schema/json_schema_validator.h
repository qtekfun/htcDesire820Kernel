// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_JSON_SCHEMA_JSON_SCHEMA_VALIDATOR_H_
#define COMPONENTS_JSON_SCHEMA_JSON_SCHEMA_VALIDATOR_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace base {
class DictionaryValue;
class ListValue;
class StringValue;
class Value;
}

class JSONSchemaValidator {
 public:
  
  struct Error {
    Error();

    explicit Error(const std::string& message);

    Error(const std::string& path, const std::string& message);

    
    std::string path;

    
    std::string message;
  };

  enum Options {
    
    
    OPTIONS_IGNORE_UNKNOWN_ATTRIBUTES = 1 << 0,
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
  static const char kInvalidTypeIntegerNumber[];

  
  static std::string GetJSONSchemaType(const base::Value* value);

  
  
  
  static std::string FormatErrorMessage(const std::string& format,
                                        const std::string& s1);
  static std::string FormatErrorMessage(const std::string& format,
                                        const std::string& s1,
                                        const std::string& s2);

  
  
  
  
  
  static scoped_ptr<base::DictionaryValue> IsValidSchema(
      const std::string& schema,
      std::string* error);

  
  
  static scoped_ptr<base::DictionaryValue> IsValidSchema(
      const std::string& schema,
      int options,
      std::string* error);

  
  
  
  
  
  explicit JSONSchemaValidator(base::DictionaryValue* schema);

  
  
  
  
  
  
  
  
  JSONSchemaValidator(base::DictionaryValue* schema, base::ListValue* types);

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

  
  
  
  bool Validate(const base::Value* instance);

 private:
  typedef std::map<std::string, const base::DictionaryValue*> TypeMap;

  
  
  

  
  
  
  void Validate(const base::Value* instance,
                const base::DictionaryValue* schema,
                const std::string& path);

  
  
  void ValidateChoices(const base::Value* instance,
                       const base::ListValue* choices,
                       const std::string& path);

  
  void ValidateEnum(const base::Value* instance,
                    const base::ListValue* choices,
                    const std::string& path);

  
  void ValidateObject(const base::DictionaryValue* instance,
                      const base::DictionaryValue* schema,
                      const std::string& path);

  
  void ValidateArray(const base::ListValue* instance,
                     const base::DictionaryValue* schema,
                     const std::string& path);

  
  
  
  void ValidateTuple(const base::ListValue* instance,
                     const base::DictionaryValue* schema,
                     const std::string& path);

  
  void ValidateString(const base::Value* instance,
                      const base::DictionaryValue* schema,
                      const std::string& path);

  
  void ValidateNumber(const base::Value* instance,
                      const base::DictionaryValue* schema,
                      const std::string& path);

  
  bool ValidateType(const base::Value* instance,
                    const std::string& expected_type,
                    const std::string& path);

  
  bool SchemaAllowsAnyAdditionalItems(
      const base::DictionaryValue* schema,
      const base::DictionaryValue** addition_items_schema);

  
  base::DictionaryValue* schema_root_;

  
  TypeMap types_;

  
  
  bool default_allow_additional_properties_;

  
  std::vector<Error> errors_;


  DISALLOW_COPY_AND_ASSIGN(JSONSchemaValidator);
};

#endif  
