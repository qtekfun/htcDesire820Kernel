// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_COMMON_SANDBOX_MAC_UNITTEST_HELPER_H_
#define CONTENT_COMMON_SANDBOX_MAC_UNITTEST_HELPER_H_

#include "base/test/multiprocess_test.h"
#include "content/common/sandbox_mac.h"

namespace content {


class MacSandboxTest : public base::MultiProcessTest {
 public:
  
  
  
  
  
  
  bool RunTestInSandbox(content::SandboxType sandbox_type,
                        const char* test_name,
                        const char* test_data);

  
  
  
  
  
  
  
  
  
  bool RunTestInAllSandboxTypes(const char* test_name,
                                const char* test_data);
};

class MacSandboxTestCase {
 public:
  virtual ~MacSandboxTestCase() {}

  
  
  
  virtual bool BeforeSandboxInit();

  
  
  
  virtual bool SandboxedTest() = 0;

  
  
  virtual void SetTestData(const char* test_data);

 protected:
  std::string test_data_;
};

namespace internal {

void AddSandboxTestCase(const char* test_name, MacSandboxTestCase* test_class);

template <class T> struct RegisterSandboxTest {
  RegisterSandboxTest(const char* test_name) {
    AddSandboxTestCase(test_name, new T);
  }
};

#define REGISTER_SANDBOX_TEST_CASE(class_name) \
  namespace { \
    content::internal::RegisterSandboxTest<class_name> \
      register_test##class_name(#class_name); \
  }  

}  

}  

#endif  
