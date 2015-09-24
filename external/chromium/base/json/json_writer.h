// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_JSON_JSON_WRITER_H_
#define BASE_JSON_JSON_WRITER_H_
#pragma once

#include <string>

#include "base/base_api.h"
#include "base/basictypes.h"

class Value;

namespace base {

class BASE_API JSONWriter {
 public:
  
  
  
  
  
  
  
  static void Write(const Value* const node, bool pretty_print,
                    std::string* json);

  
  
  
  static void WriteWithOptionalEscape(const Value* const node,
                                      bool pretty_print,
                                      bool escape,
                                      std::string* json);

  
  
  static const char* kEmptyArray;

 private:
  JSONWriter(bool pretty_print, std::string* json);

  
  
  void BuildJSONString(const Value* const node, int depth, bool escape);

  
  void AppendQuotedString(const std::string& str);

  
  void IndentLine(int depth);

  
  std::string* json_string_;

  bool pretty_print_;

  DISALLOW_COPY_AND_ASSIGN(JSONWriter);
};

}  

#endif  
