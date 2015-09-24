// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TEST_EXPECTATIONS_PARSER_H_
#define BASE_TEST_EXPECTATIONS_PARSER_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "base/test/expectations/expectation.h"

namespace test_expectations {

class Parser {
 public:
  
  
  class Delegate {
   public:
    
    
    virtual void EmitExpectation(const Expectation& expectation) = 0;

    
    
    virtual void OnSyntaxError(const std::string& message) = 0;

    
    
    
    virtual void OnDataError(const std::string& message) = 0;
  };

  
  Parser(Delegate* delegate, const std::string& input);
  ~Parser();

  
  void Parse();

 private:
  
  
  
  
  
  
  struct StateFunc;
  typedef StateFunc(Parser::*StateFuncPtr)();
  struct StateFunc {
    StateFunc(StateFuncPtr pf) : pf_(pf) {}
    operator StateFuncPtr() {
      return pf_;
    }
    StateFuncPtr pf_;
  };

  
  bool HasNext();

  
  
  
  
  StateFunc Start();
  StateFunc ParseComment();
  StateFunc ParseBugURL();
  StateFunc BeginModifiers();
  StateFunc InModifiers();
  StateFunc SaveModifier();
  StateFunc EndModifiers();
  StateFunc ParseTestName();
  StateFunc SaveTestName();
  StateFunc ParseExpectation();
  StateFunc ParseExpectationType();
  StateFunc SaveExpectationType();
  StateFunc End();

  
  
  
  StateFunc ExtractString(StateFunc success);

  
  
  StateFunc SkipWhitespace(StateFunc next);

  
  StateFunc SkipWhitespaceAndNewLines(StateFunc next);

  
  
  StateFunc SyntaxError(const std::string& message);

  
  
  void DataError(const std::string& error);

  
  Delegate* delegate_;

  
  std::string input_;

  
  const char* pos_;

  
  const char* end_;

  
  int line_number_;

  
  
  base::StringPiece extracted_string_;

  
  
  Expectation current_;

  
  
  bool data_error_;
};

}  

#endif  
