// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WIN8_TEST_UI_AUTOMATION_CLIENT_H_
#define WIN8_TEST_UI_AUTOMATION_CLIENT_H_


#include <windows.h>
#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string16.h"
#include "base/threading/thread.h"
#include "base/threading/thread_checker.h"

namespace win8 {
namespace internal {

class UIAutomationClient {
 public:
  
  
  
  
  typedef base::Callback<void(HRESULT)> InitializedCallback;

  
  
  
  typedef base::Callback<void(HRESULT, std::vector<string16>)> ResultCallback;

  UIAutomationClient();
  ~UIAutomationClient();

  
  
  
  
  void Begin(const wchar_t* class_name,
             const string16& item_name,
             const InitializedCallback& init_callback,
             const ResultCallback& result_callback);

 private:
  class Context;

  base::ThreadChecker thread_checker_;

  
  base::Thread automation_thread_;

  
  base::WeakPtr<Context> context_;

  DISALLOW_COPY_AND_ASSIGN(UIAutomationClient);
};

}  
}  

#endif  
