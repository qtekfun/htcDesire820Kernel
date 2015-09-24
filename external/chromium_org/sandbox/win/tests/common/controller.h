// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SANDBOX_WIN_TESTS_COMMON_CONTROLLER_H_
#define SANDBOX_WIN_TESTS_COMMON_CONTROLLER_H_

#include <windows.h>
#include <string>

#include "base/strings/string16.h"
#include "base/win/scoped_handle.h"
#include "sandbox/win/src/sandbox.h"

namespace sandbox {

#define SEVERITY_INFO_FLAGS   0x40000000
#define SEVERITY_ERROR_FLAGS  0xC0000000
#define CUSTOMER_CODE         0x20000000
#define SBOX_TESTS_FACILITY   0x05B10000

enum SboxTestResult {
  SBOX_TEST_FIRST_RESULT = CUSTOMER_CODE | SBOX_TESTS_FACILITY,
  SBOX_TEST_SUCCEEDED,
  SBOX_TEST_PING_OK,
  SBOX_TEST_FIRST_INFO = SBOX_TEST_FIRST_RESULT | SEVERITY_INFO_FLAGS,
  SBOX_TEST_DENIED,     
  SBOX_TEST_NOT_FOUND,  
  SBOX_TEST_FIRST_ERROR = SBOX_TEST_FIRST_RESULT | SEVERITY_ERROR_FLAGS,
  SBOX_TEST_SECOND_ERROR,
  SBOX_TEST_THIRD_ERROR,
  SBOX_TEST_FOURTH_ERROR,
  SBOX_TEST_FIFTH_ERROR,
  SBOX_TEST_SIXTH_ERROR,
  SBOX_TEST_SEVENTH_ERROR,
  SBOX_TEST_INVALID_PARAMETER,
  SBOX_TEST_FAILED_TO_RUN_TEST,
  SBOX_TEST_FAILED_TO_EXECUTE_COMMAND,
  SBOX_TEST_TIMED_OUT,
  SBOX_TEST_FAILED,
  SBOX_TEST_LAST_RESULT
};

inline bool IsSboxTestsResult(SboxTestResult result) {
  unsigned int code = static_cast<unsigned int>(result);
  unsigned int first = static_cast<unsigned int>(SBOX_TEST_FIRST_RESULT);
  unsigned int last = static_cast<unsigned int>(SBOX_TEST_LAST_RESULT);
  return (code > first) && (code < last);
}

enum SboxTestsState {
  MIN_STATE = 1,
  BEFORE_INIT,
  BEFORE_REVERT,
  AFTER_REVERT,
  EVERY_STATE,
  MAX_STATE
};

#define SBOX_TESTS_API __declspec(dllexport)
#define SBOX_TESTS_COMMAND extern "C" SBOX_TESTS_API

extern "C" {
typedef int (*CommandFunction)(int argc, wchar_t **argv);
}

class TestRunner {
 public:
  TestRunner(JobLevel job_level, TokenLevel startup_token,
             TokenLevel main_token);

  TestRunner();

  ~TestRunner();

  
  
  bool AddRule(TargetPolicy::SubSystem subsystem,
               TargetPolicy::Semantics semantics,
               const wchar_t* pattern);

  
  
  
  bool AddRuleSys32(TargetPolicy::Semantics semantics, const wchar_t* pattern);

  
  
  bool AddFsRule(TargetPolicy::Semantics semantics, const wchar_t* pattern);

  
  
  int RunTest(const wchar_t* command);

  
  void SetTimeout(DWORD timeout_ms);

  
  void SetAsynchronous(bool is_async) { is_async_ = is_async; }

  
  void SetUnsandboxed(bool is_no_sandbox) { no_sandbox_ = is_no_sandbox; }

  
  void SetTestState(SboxTestsState desired_state);

  
  
  void SetKillOnDestruction(bool value) { kill_on_destruction_ = value; }

  
  
  TargetPolicy* GetPolicy();

  BrokerServices* broker() { return broker_; }

  
  HANDLE process() { return target_process_; }

  
  DWORD process_id() { return target_process_id_; }

 private:
  
  
  void Init(JobLevel job_level, TokenLevel startup_token,
            TokenLevel main_token);

  
  int InternalRunTest(const wchar_t* command);

  BrokerServices* broker_;
  TargetPolicy* policy_;
  DWORD timeout_;
  SboxTestsState state_;
  bool is_init_;
  bool is_async_;
  bool no_sandbox_;
  bool kill_on_destruction_;
  base::win::ScopedHandle target_process_;
  DWORD target_process_id_;
};

BrokerServices* GetBroker();

base::string16 MakePathToSys(const wchar_t* name, bool is_obj_man_path);

int DispatchCall(int argc, wchar_t **argv);

}  

#endif  
