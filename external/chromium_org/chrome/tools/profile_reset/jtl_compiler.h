// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TOOLS_PROFILE_RESET_JTL_COMPILER_H_
#define CHROME_TOOLS_PROFILE_RESET_JTL_COMPILER_H_

#include <string>

#include "base/basictypes.h"

class JtlCompiler {
 public:
  struct CompileError {
    enum ErrorCode {
      ERROR_NONE,
      MISMATCHED_DOUBLE_QUOTES,
      PARSING_ERROR,
      INVALID_OPERATION_NAME,
      INVALID_ARGUMENT_COUNT,
      INVALID_ARGUMENT_TYPE,
      INVALID_ARGUMENT_VALUE
    };

    CompileError() : line_number(0), error_code(ERROR_NONE) {}
    CompileError(size_t line_number,
                 const std::string& context,
                 ErrorCode error_code)
        : line_number(line_number), context(context), error_code(error_code) {}

    size_t line_number;  
    std::string context;
    ErrorCode error_code;
  };

  
  
  
  
  
  static bool Compile(const std::string& source_code,
                      const std::string& hash_seed,
                      std::string* output_bytecode,
                      CompileError* error);

 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(JtlCompiler);
};

#endif  
