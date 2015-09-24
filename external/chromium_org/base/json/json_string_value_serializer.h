// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_JSON_JSON_STRING_VALUE_SERIALIZER_H_
#define BASE_JSON_JSON_STRING_VALUE_SERIALIZER_H_

#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/values.h"

class BASE_EXPORT JSONStringValueSerializer : public base::ValueSerializer {
 public:
  
  
  
  explicit JSONStringValueSerializer(std::string* json_string)
      : json_string_(json_string),
        initialized_with_const_string_(false),
        pretty_print_(false),
        allow_trailing_comma_(false) {
  }

  
  
  explicit JSONStringValueSerializer(const std::string& json_string)
      : json_string_(&const_cast<std::string&>(json_string)),
        initialized_with_const_string_(true),
        pretty_print_(false),
        allow_trailing_comma_(false) {
  }

  virtual ~JSONStringValueSerializer();

  
  // JSON.  If the return value is true, the result will have been written
  
  virtual bool Serialize(const base::Value& root) OVERRIDE;

  
  
  bool SerializeAndOmitBinaryValues(const base::Value& root);

  
  
  
  
  
  
  
  virtual base::Value* Deserialize(int* error_code,
                                   std::string* error_message) OVERRIDE;

  void set_pretty_print(bool new_value) { pretty_print_ = new_value; }
  bool pretty_print() { return pretty_print_; }

  void set_allow_trailing_comma(bool new_value) {
    allow_trailing_comma_ = new_value;
  }

 private:
  bool SerializeInternal(const base::Value& root, bool omit_binary_values);

  std::string* json_string_;
  bool initialized_with_const_string_;
  bool pretty_print_;  
  
  bool allow_trailing_comma_;

  DISALLOW_COPY_AND_ASSIGN(JSONStringValueSerializer);
};

#endif  

