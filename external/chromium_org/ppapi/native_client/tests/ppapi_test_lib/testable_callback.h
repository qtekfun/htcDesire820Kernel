// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NATIVE_CLIENT_TESTS_PPAPI_TEST_LIB_TESTABLE_CALLBACK_H
#define NATIVE_CLIENT_TESTS_PPAPI_TEST_LIB_TESTABLE_CALLBACK_H

#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_stdint.h"



class TestableCallback {
 public:
  TestableCallback(PP_Instance instance, bool force_async);

  
  PP_CompletionCallback GetCallback();

  
  
  
  
  
  int32_t WaitForResult();

  bool HasRun() const { return run_count_ != 0; }

  
  void Reset() {
    run_count_ = 0;
    have_result_ = false;
  }

  int32_t Result() const { return result_; }

 private:
  static void Handler(void* user_data, int32_t result);

  bool have_result_;       
  int32_t result_;         
  bool force_async_;       
  bool post_quit_task_;    
  unsigned run_count_;     
  PP_Instance instance_;
};

#endif  
