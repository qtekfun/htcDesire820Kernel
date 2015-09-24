// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TOOLS_GN_INPUT_CONVERSION_H_
#define TOOLS_GN_INPUT_CONVERSION_H_

#include <string>

class Err;
class ParseNode;
class Value;

extern const char kInputConversion_Help[];

Value ConvertInputToValue(const std::string& input,
                          const ParseNode* origin,
                          const Value& input_conversion_value,
                          Err* err);

#endif  
