// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PROFILE_RESETTER_JTL_INTERPRETER_H_
#define CHROME_BROWSER_PROFILE_RESETTER_JTL_INTERPRETER_H_

#include <string>

#include "base/basictypes.h"
#include "base/values.h"

class JtlInterpreter {
 public:
  enum Result {
    OK,
    PARSE_ERROR,
    RUNTIME_ERROR,
    RESULT_MAX,
  };

  
  
  
  
  
  JtlInterpreter(const std::string& hasher_seed,
                 const std::string& program,
                 const DictionaryValue* input);
  ~JtlInterpreter();

  void Execute();

  Result result() const { return result_; }
  const DictionaryValue* working_memory() const {
    return working_memory_.get();
  }
  bool GetOutputBoolean(const std::string& unhashed_key, bool* output) const;
  bool GetOutputString(const std::string& unhashed_key,
                       std::string* output) const;

 private:
  
  std::string hasher_seed_;
  std::string program_;
  const DictionaryValue* input_;
  
  scoped_ptr<DictionaryValue> working_memory_;
  Result result_;

  DISALLOW_COPY_AND_ASSIGN(JtlInterpreter);
};

#endif  
