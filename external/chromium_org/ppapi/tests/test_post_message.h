// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_TESTS_TEST_POST_MESSAGE_H_
#define PPAPI_TESTS_TEST_POST_MESSAGE_H_

#include <string>
#include <vector>

#include "ppapi/tests/test_case.h"

class TestPostMessage : public TestCase {
 public:
  explicit TestPostMessage(TestingInstance* instance);
  virtual ~TestPostMessage();

 private:
  
  virtual bool Init();
  virtual void RunTests(const std::string& filter);

  
  
  virtual void HandleMessage(const pp::Var& message_data);

  
  
  
  
  
  
  
  
  bool AddEchoingListener(const std::string& expression);

  
  
  bool PostMessageFromJavaScript(const std::string& func);

  
  
  
  bool ClearListeners();

  
  
  int WaitForMessages();

  
  
  
  
  
  int PostAsyncMessageFromJavaScriptAndWait(const std::string& func);

  
  
  std::string CheckMessageProperties(
      const pp::Var& test_data,
      const std::vector<std::string>& properties_to_check);

  
  
  
  std::string TestSendInInit();

  
  
  std::string TestSendingData();

  
  std::string TestSendingArrayBuffer();

  
  std::string TestSendingArray();

  
  std::string TestSendingDictionary();

  
  
  std::string TestSendingResource();

  
  std::string TestSendingComplexVar();

  
  
  std::string TestMessageEvent();

  
  std::string TestNoHandler();

  
  
  std::string TestExtraParam();

  
  std::string TestNonMainThread();

  typedef std::vector<pp::Var> VarVector;

  
  
  VarVector message_data_;
};

#endif  

