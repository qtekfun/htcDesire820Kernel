// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_TESTS_TESTING_INSTANCE_H_
#define PPAPI_TESTS_TESTING_INSTANCE_H_

#include <string>

#include "ppapi/utility/completion_callback_factory.h"

#if defined(__native_client__)
#include "ppapi/cpp/instance.h"
#else
#include "ppapi/cpp/private/instance_private.h"
#endif

#ifdef PostMessage
#undef PostMessage
#endif

class TestCase;

class TestingInstance : public
#if defined(__native_client__)
pp::Instance {
#else
pp::InstancePrivate {
#endif
 public:
  explicit TestingInstance(PP_Instance instance);
  virtual ~TestingInstance();

  
  virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]);
  virtual void DidChangeView(const pp::View& view);
  virtual bool HandleInputEvent(const pp::InputEvent& event);

#if !(defined __native_client__)
  virtual pp::Var GetInstanceObject();
#endif

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void LogTest(const std::string& test_name,
               const std::string& error_message,
               PP_TimeTicks start_time);

  
  void AppendError(const std::string& message);

  
  
  virtual void HandleMessage(const pp::Var& message_data);

  const std::string& protocol() {
    return protocol_;
  }

  int ssl_server_port() { return ssl_server_port_; }

  const std::string& websocket_host() { return websocket_host_; }
  int websocket_port() { return websocket_port_; }

  
  void EvalScript(const std::string& script);

  
  void SetCookie(const std::string& name, const std::string& value);

  void ReportProgress(const std::string& progress_value);

  
  
  void LogTestTime(const std::string& test_time);

  
  
  
  void AddPostCondition(const std::string& script);

  
  void set_remove_plugin(bool remove) { remove_plugin_ = remove; }

 private:
  void ExecuteTests(int32_t unused);

  
  
  
  TestCase* CaseForTestName(const std::string& name);

  
  void SendTestCommand(const std::string& command);
  void SendTestCommand(const std::string& command, const std::string& params);

  
  void LogAvailableTests();

  
  void LogError(const std::string& text);

  
  void LogHTML(const std::string& html);

  pp::CompletionCallbackFactory<TestingInstance> callback_factory_;

  
  TestCase* current_case_;

  
  
  std::string test_filter_;

  
  bool executed_tests_;

  
  int32_t number_tests_executed_;

  
  std::string errors_;

  
  bool nacl_mode_;

  
  
  std::string protocol_;

  
  int ssl_server_port_;

  
  std::string websocket_host_;

  
  int websocket_port_;

  
  
  
  bool remove_plugin_;
};

#endif  
