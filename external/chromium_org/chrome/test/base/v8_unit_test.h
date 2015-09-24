// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_V8_UNIT_TEST_H_
#define CHROME_TEST_BASE_V8_UNIT_TEST_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/strings/string_piece.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "v8/include/v8.h"

class V8UnitTest : public testing::Test {
 public:
  V8UnitTest();
  virtual ~V8UnitTest();

  
  virtual void SetUp() OVERRIDE;

 protected:
  
  
  void AddLibrary(const base::FilePath& library_path);

  
  bool RunJavascriptTestF(const std::string& test_fixture,
                          const std::string& test_name);

  
  
  virtual void ExecuteScriptInContext(const base::StringPiece& script_source,
                                      const base::StringPiece& script_name);

  
  virtual void SetGlobalStringVar(const std::string& var_name,
                                  const std::string& value);

  
  virtual std::string ExceptionToString(const v8::TryCatch& try_catch);

  
  
  
  
  virtual void TestFunction(const std::string& function_name);

  
  
  
  
  static void Log(const v8::FunctionCallbackInfo<v8::Value>& args);

  
  
  
  static void Error(const v8::FunctionCallbackInfo<v8::Value>& args);

  
  
  
  static void ChromeSend(const v8::FunctionCallbackInfo<v8::Value>& args);

 private:
  
  bool ExecuteJavascriptLibraries();

  
  void InitPathsAndLibraries();

  v8::Isolate* isolate_;

  
  v8::HandleScope handle_scope_;

  
  v8::Persistent<v8::Context> context_;

  
  std::vector<base::FilePath> user_libraries_;
};

#endif  
