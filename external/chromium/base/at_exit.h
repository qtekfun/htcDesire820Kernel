// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_AT_EXIT_H_
#define BASE_AT_EXIT_H_
#pragma once

#include <stack>

#include "base/base_api.h"
#include "base/basictypes.h"
#include "base/synchronization/lock.h"

namespace base {


class BASE_API AtExitManager {
 public:
  typedef void (*AtExitCallbackType)(void*);

  AtExitManager();

  
  
  ~AtExitManager();

  
  
  static void RegisterCallback(AtExitCallbackType func, void* param);

  
  
  static void ProcessCallbacksNow();

 protected:
  
  
  
  
  explicit AtExitManager(bool shadow);

 private:
  struct CallbackAndParam {
    CallbackAndParam(AtExitCallbackType func, void* param)
        : func_(func), param_(param) { }
    AtExitCallbackType func_;
    void* param_;
  };

  base::Lock lock_;
  std::stack<CallbackAndParam> stack_;
  AtExitManager* next_manager_;  

  DISALLOW_COPY_AND_ASSIGN(AtExitManager);
};

#if defined(UNIT_TEST)
class ShadowingAtExitManager : public AtExitManager {
 public:
  ShadowingAtExitManager() : AtExitManager(true) {}
};
#endif  

}  

#endif  
