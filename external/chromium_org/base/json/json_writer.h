// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_JSON_JSON_WRITER_H_
#define BASE_JSON_JSON_WRITER_H_

#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"

namespace base {

class Value;

class BASE_EXPORT JSONWriter {
 public:
  enum Options {
    
    
    OPTIONS_OMIT_BINARY_VALUES = 1 << 0,

    
    
    
    
    OPTIONS_OMIT_DOUBLE_TYPE_PRESERVATION = 1 << 1,

    
    
    OPTIONS_PRETTY_PRINT = 1 << 2,
  };

  
  
  
  
  static void Write(const Value* const node, std::string* json);

  
  
  static void WriteWithOptions(const Value* const node, int options,
                               std::string* json);

 private:
  JSONWriter(bool omit_binary_values,
             bool omit_double_type_preservation, bool pretty_print,
             std::string* json);

  
  
  void BuildJSONString(const Value* const node, int depth);

  
  void IndentLine(int depth);

  bool omit_binary_values_;
  bool omit_double_type_preservation_;
  bool pretty_print_;

  
  std::string* json_string_;

  DISALLOW_COPY_AND_ASSIGN(JSONWriter);
};

}  

#endif  
