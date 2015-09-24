// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TOOLS_PROFILE_RESET_JTL_PARSER_H_
#define CHROME_TOOLS_PROFILE_RESET_JTL_PARSER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/values.h"

class JtlParser {
 public:
  
  
  
  
  JtlParser(const std::string& compacted_source_code,
            const std::vector<size_t>& newline_indices);
  ~JtlParser();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool RemoveCommentsAndAllWhitespace(
      const std::string& verbose_text,
      std::string* compacted_text,
      std::vector<size_t>* newline_indices,
      size_t* error_line_number);

  
  
  
  bool HasFinished();

  
  
  
  
  
  bool ParseNextOperation(std::string* name,
                          base::ListValue* argument_list,
                          bool* ends_the_sentence);

  
  const std::string& compacted_source() const { return compacted_source_; }

  
  
  size_t GetOriginalLineNumber(size_t compacted_index) const;

  size_t GetLastLineNumber() const;
  std::string GetLastContext() const;

 private:
  
  
  struct ParsingState;

  std::string compacted_source_;
  std::vector<size_t> newline_indices_;
  scoped_ptr<ParsingState> state_;

  DISALLOW_COPY_AND_ASSIGN(JtlParser);
};

#endif  
