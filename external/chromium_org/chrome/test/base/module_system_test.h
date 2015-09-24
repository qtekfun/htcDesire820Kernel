// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_TEST_BASE_MODULE_SYSTEM_TEST_H_
#define CHROME_TEST_BASE_MODULE_SYSTEM_TEST_H_

#include "chrome/renderer/extensions/chrome_v8_context.h"
#include "chrome/renderer/extensions/module_system.h"
#include "chrome/renderer/extensions/scoped_persistent.h"
#include "v8/include/v8.h"
#include "testing/gtest/include/gtest/gtest.h"

class ModuleSystemTest : public testing::Test {
 public:
  ModuleSystemTest();
  virtual ~ModuleSystemTest();

  virtual void TearDown() OVERRIDE;

 protected:
  
  void RegisterModule(const std::string& name, const std::string& code);

  
  void RegisterModule(const std::string& name, int resource_id);

  
  
  
  void OverrideNativeHandler(const std::string& name, const std::string& code);

  
  
  void RegisterTestFile(const std::string& module_name,
                        const std::string& file_name);

  
  
  void ExpectNoAssertionsMade();

  
  v8::Handle<v8::Object> CreateGlobal(const std::string& name);

  v8::Isolate* isolate_;
  v8::HandleScope handle_scope_;
  scoped_ptr<extensions::ChromeV8Context> context_;
  class AssertNatives;
  AssertNatives* assert_natives_;
  class StringSourceMap;
  scoped_ptr<StringSourceMap> source_map_;
  scoped_ptr<extensions::ModuleSystem> module_system_;
  bool should_assertions_be_made_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ModuleSystemTest);
};

#endif  
